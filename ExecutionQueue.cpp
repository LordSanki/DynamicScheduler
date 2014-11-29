#include <ExecutionQueue.h>
#include <SchedulerQueue.h>

ExecutionQueue::ExecutionQueue(int size, ReorderBuffer &buf, RegisterFile &file)
              :rob(buf),rFile(file)
{
  max_size = size; bw = size;
  latency_map[0] = 1;
  latency_map[1] = 2;
  latency_map[2] = 5;
}

bool ExecutionQueue::push(ROBIndex index)
{
//  if((int)queue.size() > max_size)
  //  return false;
  queue.push_back(index);
  return true;
}

void ExecutionQueue::execute( SchedulerQueue *sQueue )
{
  for(InsList::iterator it = queue.begin();
      it != queue.end(); it++)
  {
    rob[*it].timer++;
    if( latency_expired(rob[*it]) )
    {
      rob[*it].state(WB);
      sQueue->wake(rob[*it].dest);
      rFile.update(rob[*it].dest);
      *it = -1;
    }
  }
  queue.remove_if(invalid_index);
}

bool ExecutionQueue::latency_expired(const Instruction &ins)
{
  return (latency_map[ins.op] <= ins.timer);
}

