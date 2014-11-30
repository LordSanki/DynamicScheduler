#ifndef __REPLACEMENT_POLICY_H__
#define __REPLACEMENT_POLICY_H__

#include <CustomTypes.h>
#include <TagStore.h>
#include <exception>
#include <iostream>
namespace CacheSimulator
{
  namespace ReplacementPolicy
  {
    enum Types
    {
      e_LRU=0,
      e_LFU=1
    };

    inline void initLRU(TagSet &set)
    {
      i32 counter = set.size()-1;
      for (ui32 i = 0; i<set.size(); i++)
      {
        set[i].count_block(counter--);
      }
    }
    inline void initLFU(TagSet &set)
    {
      set.count_set(0);
      for (ui32 i = 0; i<set.size(); i++)
      {
        set[i].count_block(0);
      }
    }
    inline TagEntry & findLRU(TagSet &set)
    {
      ui32 max_index = set.size() -1;
      for (ui32 i = 0; i<set.size(); i++)
      {
        if(set[i].count_block() == max_index)
          return set[i];
      }
      throw "Unable to find LRU candidate";
      return TagEntry::invalidTag();
    }
    inline TagEntry & findLFU(TagSet &set)
    {
      i32 lfu_index = 0;
      for (ui32 i = 1; i<set.size(); i++)
      {
        if(set[lfu_index].count_block() > set[i].count_block())
          lfu_index = i;
      }
      set.count_set( set[lfu_index].count_block() );
      return set[lfu_index];
    }
    inline void updateLRU(TagSet &set, TagEntry &ref)
    {
      for (ui32 i = 0; i<set.size(); i++)
      {
        if( set[i].count_block() < ref.count_block())
          set[i].count_block( set[i].count_block()+1 );
      }
      ref.count_block(0);
    }
    inline void updateLFU(TagSet &set, TagEntry &ref)
    {
      if(ref.count_block() == UNDEFINED_COUNT_BLOCK)
      {
        ref.count_block(set.count_set()+1);
      }
      else
      {
        ref.count_block(ref.count_block()+1);
      }
    }
  };
};

#endif //__REPLACEMENT_POLICY_H__

