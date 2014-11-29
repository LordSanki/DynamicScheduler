#ifndef __SCHEDULER_QUEUE_H__
#define __SCHEDULER_QUEUE_H__

#include <Instruction.h>
#include <RegisterFile.h>
class ExecutionQueue;

class SchedulerQueue
{
  public:
    SchedulerQueue(int size, int bandwidth, ReorderBuffer &buf, RegisterFile &file);

    bool push(ROBIndex index);
    void issue(ExecutionQueue *exQueue);
    void wake(int dest);
  private:
    int bw;
    int max_size;
    InsList queue;
    ReorderBuffer &rob;
    RegisterFile &rFile;
};

#endif //__SCHEDULER_QUEUE_H__

