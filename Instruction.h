#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <vector>
#include <list>
#include <RegisterFile.h>
#include <iostream>
using namespace std;
enum States
{
  IF,
  ID,
  IS,
  EX,
  WB,
  RET
};

typedef int ROBIndex;
typedef std::list<ROBIndex> InsList;
typedef std::vector<ROBIndex> InsPipe;

class Instruction
{
  public:
    States state;
    int src1;
    bool s1_ready;
    int src2;
    bool s2_ready;
    int dest;
    int op;
    int timer;
    bool ready() { return s2_ready&&s1_ready; }
    void update_src(int name)
    {
      s1_ready = s1_ready || (name == src1);
      s2_ready = s2_ready || (name == src2);
    }
    Instruction()
    {
      src1 = src2 = dest = -1;
      s1_ready = s2_ready = false;
      op = 0;
      timer = 0;
      state = IF;
    }
};

class ReorderBuffer
{
  public:
    ReorderBuffer()
    {
      offset =  REGISTER_FILE_SIZE;
      ret_index = 0;
      _size = 0;
    }
    Instruction & operator[] (int index) { return buff[index-offset]; }
    void retire()
    {
      int k=ret_index;
      for(int i=0; i<_size; i++)
      {
        if(buff[k].state == WB)
        {
          buff[k].state = RET;
          _size--;
          //buff.pop_front();
          //offset++;
          i--;
          ret_index++;
        }
        else
          break;
        k++;
      }
    }
    void push(Instruction & ins) { buff.push_back(ins); _size++; cout<<"Size:"<<_size<<endl;}
    int size() { return (ret_index + _size + offset); }
    operator bool() { return (_size > 0); }
  private:
    std::vector<Instruction> buff;
    int offset, ret_index, _size;
};


inline bool invalid_index (const int &v) { return v == -1; }

#endif //__INSTRUCTION_H__
