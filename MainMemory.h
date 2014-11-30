#ifndef __MAIN_MEMORY_H__
#define __MAIN_MEMORY_H__

#include <Memory.h>
#include <CustomTypes.h>
#include <iostream>
namespace CacheSimulator
{
  class MainMemory: public Memory
  {
    public:
      MainMemory()
      {
        _next = NULL;
        name("MainMemory");
        _valid = true;
      }
      ~MainMemory() 
      {
      }
      //function to initialize memory
      void initC()
      {
      }
      // function to read data
      void readC(ui32 addr, ui8 &data)
      {
      }
      // function to write data
      void writeC(ui32 addr, ui8 data)
      {
      }
  };
};

#endif //__MAIN_MEMORY_H__

