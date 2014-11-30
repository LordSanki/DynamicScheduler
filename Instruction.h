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
    struct TimingStats
    {
      int start;
      int end;
      TimingStats() {start = -1; end = -1;}
    };

  public:
    int src1,s1;
    bool s1_ready;
    int src2,s2;
    bool s2_ready;
    int dest,d;
    int op;
    int timer;
    unsigned long mem;
  private:
    static int clock;
    States _state;
    std::map<States,TimingStats> tStats;

  public:
    bool ready();
    void update_src(int name);
    Instruction(int op_, int s1_, int s2_,
                     int d_, unsigned long m_);
    Instruction();
    void state(States s);
    States state();
    const TimingStats & operator[] (States);
    static int cycles();
    static void tick();
};

class ReorderBuffer
{
  public:
    ReorderBuffer();
    Instruction & operator[] (int index);
    void retire();
    void push(Instruction & ins);
    int size();
    operator bool();
    void prepare_result();
  private:
    std::vector<Instruction> buff;
    int offset, ret_index, _size;
};


inline bool invalid_index (const int &v) { return v == -1; }

#endif //__INSTRUCTION_H__
