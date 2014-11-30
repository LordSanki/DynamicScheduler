#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <string>
#include <CustomTypes.h>
#include <iostream>
# define DATA 255U

namespace CacheSimulator
{
  class Memory
  {
    public:
      // initializing function setting read and write counter to 0
      void init ()
      {
        _reads = 0;
        _writes = 0;
        if(_valid)
          initC ();
        if(_next)
          _next->init();
      }

      // function to read data
      ui8 read(ui32 addr)
      {
        ui8 data = _data;
        if(_valid)
        {
          _reads++;
          readC (addr, data);
        }
        else if (_next)
        {
          data = _next->read(addr);
        }
        return data;
      }
      // function to write data
      void write(ui32 addr, ui8 data = DATA)
      {
        if (_valid)
        {
          _writes++;
          writeC (addr, data);
        }
        else if (_next)
          return _next->write(addr, data);
        else
          return;
      }

      void setData(ui8 data) {_data = data;}
      // getter for reads
      ui32 reads () const  { return _reads;  }
      // getter for writes
      ui32 writes () const { return _writes; }
      // getter for name
      std::string name () const { return _name; }
      // setter for name
      void name (std::string n) { _name = n;    }
      // read accessor for next
      Memory *next() const { return _next; }
      // vitual desctructor
      virtual ~Memory()
      {
        if(_next)
          delete _next;
      }
      virtual bool isCache() {return false;}
      operator bool() {return _valid;}

    protected:
      // abstract function for initialization of memory
      virtual void initC () = 0;
      // abstract function for customized reads
      virtual void readC (ui32 addr, ui8 &data) = 0;
      // abstract function for customized writes
      virtual void writeC (ui32 addr, ui8 data) = 0;
      // pointer to object of next memory
      Memory *_next;
      //flag to store if cache configuration is valid
      bool _valid;

    private:
      // counter for tracking reads
      ui32 _reads;
      // counter for tracking writes
      ui32 _writes;
      // name of memory
      std::string _name;
      // fake data to be read
      ui8 _data;
  };
};

#endif //__MEMORY_H__

