#ifndef __DISPATCH_QUEUE_H__
#define __DISPATCH_QUEUE_H__

#include <Instruction.h>
#include <TraceReader.h>

class SchedulerQueue;

class DispatchQueue
{
  public:
    DispatchQueue(int size, ReorderBuffer &buf, TraceReader &reader);
    void fetch();
    void dispatch(SchedulerQueue *sQueue);
    int num_instructions(){return num_ins;}

  private:
    int num_ins;
    int max_size;
    int bw;
    ReorderBuffer &rob;
    InsList queue;
    TraceReader &tr;

  private:
    bool push(ROBIndex index);
    ROBIndex pop();
    ROBIndex peek();
};

#endif //__DISPATCH_QUEUE_H__

