#ifndef __TRACE_READER_H__
#define __TRACE_READER_H__
#include <cstdio>
#include <exception>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cstring>
#define INVALID_CHAR 255U
class TraceReader
{
  typedef unsigned char ui8;
  typedef char c8;
  typedef unsigned long ui64;
  typedef unsigned short ui16;
  private:
  FILE *_fp;
  ui64 _nAddr, _nMem;
  int _nDest, _nSrc1, _nSrc2, _nOp;

  ui16 _hexMap[256];
  ui64 Ascii2Hex(ui8 buff [])
  {
    ui64 num=0U;
    int k=0;
    while(buff[k])
    {
      if(_hexMap[(int)buff[k]] != INVALID_CHAR)
      {
        num = (num<<4U) + _hexMap[(int)buff[k]];
      }
      k++;
    }
    return num;
  }
  void parse()
  {
    using namespace std;
    c8 buff[200] = {0};
    _nAddr = 0U;
    if(!feof(_fp))
    {
      fgets(buff, 200, _fp);

      // parsing PC
      ui8 * addr = ((ui8*)strtok(buff, " \t"));
      if(addr)
        _nAddr = Ascii2Hex(addr);

      // parsing OPCode
      c8 * str = strtok(NULL," \t");
      if(str)
        _nOp = atoi(str);

      // parsing Destination
      str = strtok(NULL," \t");
      if(str)
        _nDest = atoi(str);

      // parsing Source1
      str = strtok(NULL," \t");
      if(str)
        _nSrc1 = atoi(str);

      // parsing Source2
      str = strtok(NULL," \t");
      if(str)
        _nSrc2 = atoi(str);

      // parsing Mem address
      addr = ((ui8*)strtok(NULL, " \n\r\t"));
      if(str)
        _nMem = Ascii2Hex(addr);

    }
  }
  public:
  TraceReader(c8 *filename)
    :_fp(NULL)
  {
    for(ui16 i=1; i<256;i++)
      _hexMap[i] = INVALID_CHAR;
    _hexMap[0] = 0;
    ui8 val=0;
    for (ui8 i='0'; i<='9'; i++)
      _hexMap[i] = val++;
    val=0xa;
    for(ui8 i='a'; i<='f'; i++)
      _hexMap[i] = val++;
    val=0xa;
    for(ui8 i='A'; i<='F'; i++)
      _hexMap[i] = val++;

    _fp = fopen(filename, "rb");
    if(_fp == NULL) throw "Unable to open trace file";
    parse();
  }
  ~TraceReader()
  {
    if(_fp)
      fclose(_fp);
    _fp = NULL;
  }
  operator bool() const
  {
    return (_nAddr != 0U);
  }
  void operator ++(int)
  {
    parse();
  }
  ui64 pc() const {return _nAddr;}
  ui64 mem() const {return _nMem;}
  int dest() const {return _nDest;}
  int s1() const {return _nSrc1;}
  int s2() const {return _nSrc2;}
  int op() const {return _nOp;}
};
#endif //__TARCE_READER_H__
