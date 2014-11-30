#include <DispatchQueue.h>
#include <SchedulerQueue.h>
#include <iostream>
using namespace std;
DispatchQueue::DispatchQueue(int size, ReorderBuffer &buf, TraceReader &reader)
             :rob(buf),tr(reader)
{
  max_size = 2*size;
  bw = size;
  num_ins=0;
}
void DispatchQueue::fetch()
{
  // advanceing all previous instructions to ID
  for(InsList::iterator it = queue.begin();
      it!= queue.end(); it++)
  {
    if(rob[*it].state() == IF)
      rob[*it].state(ID);
  }

  // fetching from trace file and adding entry in fake
  // reorder buffer(rob)
  for(int i=0; i<bw; i++)
  {
    if(!tr) break;
    if(push(rob.size()))
    {
      Instruction ins(tr.op(),tr.s1(),tr.s2(),tr.dest(), tr.mem());
      ins.state(IF);
      rob.push(ins);
      tr++;
    }
    else
      break;
  }
}

void DispatchQueue::dispatch(SchedulerQueue *sQueue)
{
  int bw_left = bw;
  // advancing all instructions in ID stage to IS stage
  // maximum of N instructions can be moved fwd
  for(InsList::iterator it = queue.begin();
      (it != queue.end())&&(bw_left>0); it++)
  {
    if(invalid_index(*it)) break;
    if(rob[*it].state() == ID)
    {
      if(sQueue->push(*it))
      {
        rob[*it].state(IS);
        *it = -1;
        bw_left--;
      }
    }
  }
  queue.remove_if(invalid_index);
}

// pushing index into queue if size is less than 2N
bool DispatchQueue::push(ROBIndex index)
{
  if((int)queue.size() < max_size)
  {
    num_ins++;
    queue.push_back(index);
    return true;
  }
  return false;
}

