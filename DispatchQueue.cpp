#include <DispatchQueue.h>
#include <SchedulerQueue.h>

DispatchQueue::DispatchQueue(int size, ReorderBuffer &buf, TraceReader &reader)
             :rob(buf),tr(reader)
{
  max_size = size;
  bw = size;
}
void DispatchQueue::fetch()
{
  for(InsList::iterator it = queue.begin();
      it!= queue.end(); it++)
  {
    rob[*it].state = ID;
  }

  for(int i=0; i<bw; i++)
  {
    if(!tr) break;
    if(push(rob.size()))
    {
      Instruction ins;
      ins.state = IF;
      ins.src1  = tr.s1();
      ins.src2  = tr.s2();
      ins.dest  = tr.dest();
      ins.op    = tr.op();
      rob.push(ins);
    }
    else
      break;
  }
}
void DispatchQueue::dispatch(SchedulerQueue *sQueue)
{
  for(int i=0; i<bw; i++)
  {
    int index = peek();
    if(invalid_index(index))
      break;
    else if(rob[index].state != ID)
      break;
    else if(sQueue->push(index))
      (void)pop();
    else
      break;
  }
}

bool DispatchQueue::push(ROBIndex index)
{
  if((int)queue.size() < max_size)
  {
    queue.push_back(index);
    return true;
  }
  return false;
}
ROBIndex DispatchQueue::pop()
{
  if(queue.size() > 0)
  {
    ROBIndex index = queue.front();
    queue.pop_front();
    return index;
  }
  return -1;
}
ROBIndex DispatchQueue::peek()
{
  if(queue.size() > 0)
  {
    ROBIndex index = queue.front();
    return index;
  }
  return -1;
}

