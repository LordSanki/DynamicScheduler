#ifndef __TAG_STORE_H__
#define __TAG_STORE_H__

#include <vector>
#include <list>
#include <CustomTypes.h>
#include <exception>
#include <iostream>
namespace CacheSimulator
{
#define UNDEFINED_COUNT_BLOCK 0xfffffffe
  class TagEntry
  {
    public:
      TagEntry(ui32 tag) 
      {
        _dirty = false;
        _valid = true;
        _tag = tag;
        _count_block = UNDEFINED_COUNT_BLOCK;
      }
      TagEntry()
      {
        _valid = false;
        _dirty = false;
        _tag = 0U;
        _count_block = UNDEFINED_COUNT_BLOCK;
      }
      static TagEntry & invalidTag()
      {
        static TagEntry null;
        null = TagEntry();
        return null;
      }
      static void swap(TagEntry &t1, TagEntry& t2)
      {
        TagEntry t3 = t2;
        t2._valid = t1._valid;
        t2._dirty = t1._dirty;
        t2._tag = t1._tag;
        t1._tag = t3._tag;
        t1._valid = t3._valid;
        t1._dirty = t3._dirty;
      }
      ui32 tag() const {return _tag;}
      void tag(ui32 addr) {_tag = addr;}
      void read() { }
      void write(bool flag) { _dirty = flag; }
      bool dirty() const { return _dirty; }
      operator bool() const { return _valid; }
      bool operator ==(const TagEntry &other) const
      {
        return (_tag == other.tag());
      }
      bool operator ==(ui32 tag) const
      {
        return (_tag == tag);
      }
      ui32 count_block() const {return _count_block;}
      void count_block(ui32 hist) {_count_block = hist;}
      void reset(ui32 tag) 
      {
        _tag = tag;
        _valid = true;
        _dirty = false;
      }
      void copy(TagEntry &other)
      {
        TagEntry t = other;
        TagEntry::swap(t, *this);
      }
    private:
      ui32 _count_block;
      ui32 _tag;
      bool _valid;
      bool _dirty;
  };
  
  class TagSet : public std::vector<TagEntry>
  {
    public:
      TagSet(ui32 count=0):std::vector<TagEntry>(count) { _count_set = 0; }
      ui32 count_set() const { return _count_set; }
      void count_set(ui32 val) { _count_set = val; }
      void sort()
      {
        quick_sort(0,size()-1);
//          bubble_sort();
      }
    private:
      ui32 _count_set;
      void bubble_sort()
      {
        for (ui32 i=0; i<size()-1; i++)
        {
          for(ui32 j=i+1; j<size(); j++)
          {
            if(at(i).count_block() > at(j).count_block())
            {
              TagEntry t = at(i);
              at(i) = at(j);
              at(j) = t;
            }
          }
        }
      }
      void quick_sort (i32 start, i32 end)
      {
        i32 n = end - start;
        if (n < 1)
          return;
        i32 pivot = start + n/2;
        ui32 p = at(pivot).count_block();
        i32 s = start;
        i32 e = end;
        while (s <= e)
        {
          if ( at(s).count_block() < p)
            s++;
          else if ( at(e).count_block() > p) 
            e--;
          else
          {
            TagEntry t = at(s);
            at(s) = at(e);
            at(e) = t;
            s++;
            e--;
          }
        }
        quick_sort(start, e);
        quick_sort(s, end);
      }
  }; 
  typedef std::vector<TagSet> TagStore;

};

#endif //__TAG_STORE_H__

