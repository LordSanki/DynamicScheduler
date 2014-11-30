#ifndef __ADDRESS_DECODER_H__
#define __ADDRESS_DECODER_H__

#include <CustomTypes.h>

# define DEFAULT_ADDR_SIZE    32U
namespace CacheSimulator
{
  class AddressDecoder
  {
    public:
      AddressDecoder()
      {
        _initialized = false;
      }
      // parameterized constructor for variable address size
      AddressDecoder(ui16 sets, ui16 block_size, ui8 address_size)
      {
        init(sets, block_size, address_size);
        _initialized = true;
      }
      // parameterized constructor assuming 32 bit addresses
      AddressDecoder(ui16 sets, ui16 block_size)
      {
        init(sets, block_size, DEFAULT_ADDR_SIZE);
        _initialized = true;
      }
      // function to get tag
      ui32 tag(ui32 addr)
      {
        return (addr&_tagMask)>>(_boBits+_indexBits);
      }
      // function to get index
      ui32 index(ui32 addr)
      {
        return (addr&_indexMask)>>_boBits;
      }
      // function to get block offset
      ui32 bo(ui32 addr)
      {
        return addr&_boMask;
      }
      ui32 tag_index(ui32 addr)
      {
        return addr>>_boBits;
      }
      //function to generate address by combining tag index and blockoffset
      ui32 generate(ui32 tag, ui32 index, ui32 bo=0)
      {
        return (tag<<(_boBits+_indexBits))|(index<<_boBits)|bo;
      }
      ui32 assemble(ui32 tag, ui32 index)
      {
        return (tag<<_indexBits)|index;
      }
      ui32 disassemble(ui32 tag)
      {
        return (tag>>_indexBits);
      }

      operator bool() { return _initialized; }
    private:
      // no of tag bits = address_size - (index_bits + blockoffset_bits)
      ui16 _tagBits;
      // no of index bits = log2(sets)
      ui16 _indexBits;
      // no of blockoffset bits = log2(block_size)
      ui16 _boBits;
      // bit mask for extracting tag bits
      ui32 _tagMask;
      // bit mask for extracting index bits
      ui32 _indexMask;
      // bit mask for extracting blockoffset bits
      ui32 _boMask;
      // flag to check if AddressDecoder is initialized.
      bool _initialized;
    private:
      ui16 find_log2(ui16 val)
      {
        ui16 count = 1;
        ui16 i;
        for(i=0; i<32; i++)
        {
          if(count == val)
            break;
          count = count*2;
        }
        return i;
      }
      void init(ui16 sets, ui16 block_size, ui8 address_size)
      {
        _boBits = find_log2(block_size);
        _indexBits = find_log2(sets);
        _tagBits = address_size - (_boBits + _indexBits);

        // generating bitmask for block offset
        _boMask = 0U;
        for(ui8 i=0U; i<_boBits; i++)
        {
          _boMask = (_boMask<<1U)+ 1U;
        }
        
        // generating bitmask for index
        _indexMask = 0U;
        for(ui8 i=0; i<_indexBits; i++)
        {
          _indexMask = (_indexMask<<1U) + 1U;
        }
        _indexMask = _indexMask<<_boBits;

        // generating bitmask for tag
        _tagMask = 0U;
        for(ui8 i=0; i<_tagBits; i++)
        {
          _tagMask = (_tagMask<<1U) + 1U;
        }
        _tagMask = _tagMask<<(_boBits+_indexBits);
      }
  };
};

#endif //__ADDRESS_DECODER_H__

