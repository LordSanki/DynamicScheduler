#ifndef __CACHE_H__
#define __CACHE_H__

#include <Memory.h>
#include <CustomTypes.h>
#include <AddressDecoder.h>
#include <iostream>
#include <TagStore.h>
#include <ReplacementPolicy.h>
#include <cassert>
namespace CacheSimulator
{
  enum WritePolicy
  {
    e_WBWA=0,
    e_WTNA=1
  };
  class Cache: public Memory
  {
    friend class ResultGenerator;
    public:
    Cache(ui16 block_size, ui16 size, ui16 assoc,
        ReplacementPolicy::Types rPol, WritePolicy wPol, Memory *next, ui32 victimCacheSize = 0)
      :_victimCache(NULL)
    {
      // cache is enabled
      if(assoc > 0U && block_size > 0U)
      {
        _set = size/(block_size*assoc);
        _addrDec = AddressDecoder(_set, block_size);
        _valid = true;
      }
      // cache is disabled
      else
      {
        _set = 0U;
        _valid = false;
      }

      _next = next;
      _rPolicy = rPol;
      _wPolicy = wPol;
      _size = size;
      _assoc = assoc;
      _blocksize = block_size;
      _victimSize = victimCacheSize;
    }
    ~Cache()
    {
      // cleanup
      if(_victimCache)
        delete _victimCache;
    }
    // read accessors
    ui32 size() const {return _size;}
    ui16 assoc() const {return _assoc;}
    ui16 blocksize() const {return _blocksize;}
    ui16 set() const {return _set;}
    ui32 rhits() const {return _rhits;}
    ui32 whits() const {return _whits;}
    ui32 rPolicy() const {return (ui32)_rPolicy;}
    ui32 wPolicy() const {return (ui32)_wPolicy;}
    ui32 wbacks() const {return _wbacks;}
    ui32 vswaps() const {return _victimSwaps;}
    ui32 vwbacks() const {return _victimWBacks;}
    bool hasVictimCache() { if(_victimCache) return true; return false; }
    virtual bool isCache() {return true;}
    TagSet & tags(i32 i) {return _tags[i];}
    protected:
    // function to read data
    void readC(ui32 addr, ui8 &data)
    {
      // check if entry is present in cache
      if(cacheHit(addr))
      {
        _rhits++;
      }
      // issue read miss if enrty is missing
      else
      {
        if(_victimCache)
        {
          if(victimHit(addr))
            _rhits++;
          else
            victimMiss(addr);
        }
        else
          cacheMiss(addr, data);
      }
      return;
    }
    // function to write data
    void writeC(ui32 addr, ui8 data)
    {
      // chechk if enrty is present
      TagEntry &tag = cacheHit(addr);
      bool dirty = true;
      switch(_wPolicy)
      {
        case e_WBWA:
          dirty = true;
          // case when entry is not present
          if(!tag)
          {
            // issue a read miss if enrty is not present
            // this will bring the enrty into cache
            // now write data and set dirty bit of new enrty
            if(_victimCache)
            {
              TagEntry &t = victimHit(addr);
              if(t)
              {
                t.write(dirty);
                _whits++;
              }
              else
              {
                victimMiss(addr).write(dirty);
              }
            }
            else
              cacheMiss(addr, data).write(dirty);
          }
          break;
        case e_WTNA:
          dirty = false;
          // issue write to next level ie write through
          _next->write(addr, data);
          break;
        default:
          throw "Invalid Write Policy";
          break;
      }
      // if entry is present in cache then set dirty bit according to write policy
      if(tag)
      {
        _whits++;
        tag.write(dirty);
      }
    }
    void initC()
    {
      _rhits = 0;
      _whits = 0;
      _wbacks = 0;
      _victimWBacks = 0;
      _victimSwaps = 0;
      // create associative sets
      TagSet set(_assoc);

      // Initializing the victim cache
      if(_victimSize > 0 )
        _victimCache = new TagSet(_victimSize/_blocksize);

      // initialize the set according to replacement policy
      switch(_rPolicy)
      {
        case ReplacementPolicy::e_LRU:
          ReplacementPolicy::initLRU(set);
          // initialize victim cache according to replacement policy
          if(_victimSize) ReplacementPolicy::initLRU(*_victimCache);
          break;
        case ReplacementPolicy::e_LFU:
          ReplacementPolicy::initLFU(set);
          // initialize victim cache according to replacement policy
          if(_victimSize) ReplacementPolicy::initLFU(*_victimCache);
          break;
        default:
          throw "Invalid Read Policy";
          break;
      }
      // create array of sets
      for(ui32 i=0; i<_set; i++)
        _tags.push_back(set);

    }
    private:
    // total bytes of data store
    ui32 _size;
    //  the associativity of the cache
    ui16 _assoc;
    // number of bytes in a block
    ui16 _blocksize;
    // replacement policy
    ReplacementPolicy::Types _rPolicy;
    // write policy
    WritePolicy _wPolicy;
    // no of set = size/(assoc*blocksize)
    ui16 _set;
    // size of associated victim cache
    ui32 _victimSize;
    // counter for read hits
    ui32 _rhits;
    // counter for write hits
    ui32 _whits;
    // counter for writebacks
    ui32 _wbacks;
    // counter for victim swaps
    ui32  _victimSwaps;
    // counter for victim writebacks
    ui32 _victimWBacks;

    // pointer to object of AddressDecoder
    AddressDecoder _addrDec;
    // tag store memory
    TagStore _tags;
    // Using a tag set as a victim cache as victim cache is always fully assoiative
    TagSet *_victimCache;
    
    private:
    TagEntry & cacheMiss(ui32 addr, ui8 &data)
    {
      // extracting tag set according to index
      TagSet &set = _tags[_addrDec.index(addr)];
      // finding entry in cache to swap or replace
      TagEntry & cacheEntry = findEntryToReplace(set);

      // writeback is issued if cache entry is dirty
      if (cacheEntry.dirty() && cacheEntry)
      {
        _wbacks++;
        ui32 wb_addr = _addrDec.generate(cacheEntry.tag(), _addrDec.index(addr));
        _next->write(wb_addr);
      }
      // reading data from next level memory
      data = _next->read(addr);
      // updating data in cache
      cacheEntry.reset(_addrDec.tag(addr));
      // updating replacement counters
      updateAccessHistory(set, cacheEntry);
      return cacheEntry;
    }
    TagEntry & cacheHit(ui32 addr)
    {
      // extracting tag set according to index
      TagSet & set =  _tags[_addrDec.index(addr)];
      // finding tag entry in set
      TagEntry & hitEntry = findTagEntryInSet(set, _addrDec.tag(addr));
      // if entry is found the update replacement counters
      if (hitEntry)
        updateAccessHistory(set, hitEntry);
      return hitEntry;
    }
    TagEntry & victimHit(ui32 addr)
    {
      // looking for entry in victim cache
      TagEntry & victimEntry = findTagEntryInSet(*_victimCache, _addrDec.tag_index(addr) );
      // if entry is found in victim cache
      if(victimEntry)
      {
        // extracting tag set according to index
        TagSet &set = _tags[_addrDec.index(addr)];
        // finding entry in cache to swap or replace
        TagEntry & cacheEntry = findEntryToReplace(set);

        _victimSwaps++;

        // performing the swap
        TagEntry::swap(victimEntry, cacheEntry);
        // converting between tagstore of victim and current cache
        // the victim cache is fully associative hence does not have index bits
        // and these index bits are included in its tagbits itself
        // hence correcting the tag entry fileds
        victimEntry.tag(_addrDec.assemble(victimEntry.tag(), _addrDec.index(addr)));
        cacheEntry.tag(_addrDec.disassemble(cacheEntry.tag()));

        // updating replacement counter for both caches
        updateAccessHistory(*_victimCache, victimEntry);
        // updating replacement counters
        updateAccessHistory(set, cacheEntry);

        return cacheEntry;
      }
      return TagEntry::invalidTag();
    }

    // case when etry not found in victim cache also
    TagEntry & victimMiss(ui32 addr)
    {
      // extracting tag set according to index
      TagSet &set = _tags[_addrDec.index(addr)];
      // finding entry in cache to swap or replace
      TagEntry & cacheEntry = findEntryToReplace(set);

      // checking to see if cache entry is valid
      if(cacheEntry)
      {
        // finding entry to evict from victim cache
        TagEntry & evictedVictimEntry = findEntryToReplace(*_victimCache);
        // Writeback issued from victim cache if evicted block is dirty
        if(evictedVictimEntry.dirty() && evictedVictimEntry)
        {
          _victimWBacks++;
          _next->write(_addrDec.generate(0, evictedVictimEntry.tag()) );
        }
        // moving evicted block from cache to victim cache
        evictedVictimEntry.copy(cacheEntry);
        // correcting tag entry
        evictedVictimEntry.tag(_addrDec.assemble(evictedVictimEntry.tag(), _addrDec.index(addr)) );
        // updating counters
        updateAccessHistory(*_victimCache, evictedVictimEntry);
      }
      // bringing data from next level memory
      _next->read(addr);
      // saving to cache
      cacheEntry.reset(_addrDec.tag(addr));
      updateAccessHistory(set, cacheEntry);
      return cacheEntry;
    }
    // update replacement counters according to replacement policy
    void updateAccessHistory(TagSet & set, TagEntry &ref)
    {
      switch(_rPolicy)
      {
        case ReplacementPolicy::e_LRU:
          ReplacementPolicy::updateLRU(set, ref);
          break;
        case ReplacementPolicy::e_LFU:
          ReplacementPolicy::updateLFU(set, ref);
          break;
        default:
          throw "Invalid replacement Policy";
      }
    }
    // find candidate entry for eviction according to replacement policy
    TagEntry & findEntryToReplace(TagSet & set)
    {
      switch(_rPolicy)
      {
        case ReplacementPolicy::e_LRU:
          return ReplacementPolicy::findLRU(set);
          break;
        case ReplacementPolicy::e_LFU:
          return ReplacementPolicy::findLFU(set);
          break;
        default:
          throw "Invalid replacement Policy";
      }
      return TagEntry::invalidTag();
    }
    TagEntry & findTagEntryInSet(TagSet &set, ui32 tagAddr )
    {
      // iterating over set to match the tag entry
      for( ui32 i=0; i<set.size(); i++)
      {
        if( (set[i].tag() == tagAddr) && set[i] )
        {
          //return tag if found
          return set[i];
        }
      }
      // return invalid tag if not found
      return TagEntry::invalidTag();
    }
  };
};

#endif //__CACHE_H__

