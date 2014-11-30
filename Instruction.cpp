#include <Instruction.h>
#include <iostream>
using namespace std;

int Instruction::clock;

/*Instruction class*/
Instruction::Instruction(int op_, int s1_, int s2_,
                               int d_, unsigned long m_)
{
  s1 = src1 = s1_; s2 = src2 = s2_; d = dest = d_;
  s1_ready = s2_ready = false;
  op = op_;
  timer = 0;
  mem = m_;
  _state = IF;
  TimingStats s;
  tStats.insert(pair<States,TimingStats>(IF,s));
  tStats.insert(pair<States,TimingStats>(ID,s));
  tStats.insert(pair<States,TimingStats>(IS,s));
  tStats.insert(pair<States,TimingStats>(EX,s));
  tStats.insert(pair<States,TimingStats>(WB,s));
  tStats.insert(pair<States,TimingStats>(RET,s));
}

Instruction::Instruction()
{
  s1 = s2 = d = src1 = src2 = dest = -1;
  s1_ready = s2_ready = false;
  op = 0;
  timer = 0;
  mem = 0;
  _state = IF;
  TimingStats s;
  tStats.insert(pair<States,TimingStats>(IF,s));
  tStats.insert(pair<States,TimingStats>(ID,s));
  tStats.insert(pair<States,TimingStats>(IS,s));
  tStats.insert(pair<States,TimingStats>(EX,s));
  tStats.insert(pair<States,TimingStats>(WB,s));
  tStats.insert(pair<States,TimingStats>(RET,s));
}

bool Instruction:: ready() 
{
  return s2_ready&&s1_ready; 
}

void Instruction::update_src(int name)
{
  s1_ready = s1_ready || (name == src1);
  s2_ready = s2_ready || (name == src2);
}

void Instruction::state(States s)
{
  tStats[_state].end = clock;
  _state = s;
  tStats[_state].start = clock;
}

States Instruction::state()
{
  return _state;
}

const Instruction::TimingStats & Instruction::operator[] (States s)
{
  return tStats[s];
}

int Instruction::cycles()
{
  return clock;
}

void Instruction::tick()
{
  clock++;
}

/* Reorder Buffer class*/
ReorderBuffer::ReorderBuffer()
{
  offset =  REGISTER_FILE_SIZE;
  ret_index = 0;
  _size = 0;
}

Instruction & ReorderBuffer::operator[] (int index) 
{
  return buff[index-offset]; 
}

void ReorderBuffer::retire()
{
  //cout<<"RET_INDEX:"<<ret_index<<" _SIZE:"<<_size<<" SIZE():"<<size()<<endl;
  int k=ret_index;
  int ret_count = 0;
  for(int i=0; i<_size; i++)
  {
    if(buff[k].state() == WB)
    {
      buff[k].state(RET);
    }
    k++;
  }
  k=ret_index;
  for(int i=0; i<_size; i++)
  {
    if(buff[k].state() == RET)
      ret_count++;
    else
      break;
    k++;
  }
  ret_index += ret_count;
  _size -= ret_count;
}

void ReorderBuffer::push(Instruction & ins) 
{
  buff.push_back(ins); _size++;
}

int ReorderBuffer::size() 
{
  return (ret_index + _size + offset); 
}

ReorderBuffer::operator bool() 
{
  return (_size > 0); 
}

void ReorderBuffer::prepare_result()
{
  ret_index = 0;
  _size = buff.size();
  offset = 0;
}

