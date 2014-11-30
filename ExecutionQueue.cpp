#include <ExecutionQueue.h>
#include <SchedulerQueue.h>
#include <Cache.h>
using namespace CacheSimulator;

ExecutionQueue::ExecutionQueue(int size, ReorderBuffer &buf, RegisterFile &file, Cache *c)
              :rob(buf),rFile(file)
{
  cache = c;
  max_size = size; bw = size;
  latency_map[0] = 1;
  latency_map[1] = 2;
  latency_map[2] = 5;
}

bool ExecutionQueue::push(ROBIndex index)
{
  if(cache && (rob[index].op == 2))
    rob[index].timer = cache->read(rob[index].mem);
  else
    rob[index].timer = latency_map[rob[index].op];
  queue.push_back(index);
  return true;
}

void ExecutionQueue::execute( SchedulerQueue *sQueue )
{
  for(InsList::iterator it = queue.begin();
      it != queue.end(); it++)
  {
    if(latency_expired(rob[*it]))
    {
      rob[*it].state(WB);
      sQueue->wake(rob[*it].dest);
      rFile.update(rob[*it].dest);
      *it = -1;
    }
  }
  queue.remove_if(invalid_index);
}

bool ExecutionQueue::latency_expired(Instruction &ins)
{
  return ((--ins.timer) == 0);
}

