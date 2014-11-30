#ifndef __EXECUTION_QUEUE_H__
#define __EXECUTION_QUEUE_H__

#include <Instruction.h>
#include <RegisterFile.h>
class SchedulerQueue;
namespace CacheSimulator {
  class Cache;
}

class ExecutionQueue
{
  public:
    ExecutionQueue(int size, ReorderBuffer &buf,
                     RegisterFile &file, CacheSimulator::Cache *c=NULL);
    bool push(ROBIndex index);
    void execute( SchedulerQueue *sQueue );
  private:
    int max_size, bw;
    ReorderBuffer &rob;
    RegisterFile &rFile;
    InsList queue;
    int latency_map[3];
    CacheSimulator::Cache * cache;
    bool latency_expired(Instruction &ins);
};

#endif //__EXECUTION_QUEUE_H__
