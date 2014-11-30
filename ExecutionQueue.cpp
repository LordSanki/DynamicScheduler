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
  // query to cache to get latency of instruction
  if(cache && (rob[index].op == 2))
    rob[index].timer = cache->read(rob[index].mem);
  else
    // if cache not present using static mapping of latency
    rob[index].timer = latency_map[rob[index].op];
  queue.push_back(index);
  return true;
}

void ExecutionQueue::execute( SchedulerQueue *sQueue )
{
  // moving all finished instructions to WB stage
  for(InsList::iterator it = queue.begin();
      it != queue.end(); it++)
  {
    // checking if latency has expired
    if(latency_expired(rob[*it]))
    {
      rob[*it].state(WB);
      // waking up instruction in scheduler que
      sQueue->wake(rob[*it].dest);
      // updating register file
      rFile.update(rob[*it].dest);
      *it = -1;
    }
  }
  queue.remove_if(invalid_index);
}

// counting down the timer and checking if it hits 0
bool ExecutionQueue::latency_expired(Instruction &ins)
{
  return ((--ins.timer) == 0);
}

