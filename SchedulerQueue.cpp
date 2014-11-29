#include <Instruction.h>
#include <RegisterFile.h>
#include <ExecutionQueue.h>
#include <SchedulerQueue.h>

SchedulerQueue::SchedulerQueue(int size, int bandwidth, ReorderBuffer &buf, RegisterFile &file)
              :rob(buf),rFile(file)
{
  max_size = size; bw = bandwidth; queue.clear();
}

//int space() { return max_size - queue.size(); }
bool SchedulerQueue::push(ROBIndex index) 
{
  if ((int)queue.size() < max_size)
  {
    queue.push_back(index);

    // renaming src and destination registers
    Instruction &ins = rob[index];
    ins.s1_ready = rFile[ins.src1].ready;
    ins.src1 = rFile[ins.src1].name;
    ins.s2_ready = rFile[ins.src2].ready;
    ins.src2 = rFile[ins.src2].name;
    ins.dest = rFile.rename(ins.dest, index);
    return true;
  }
  else
    return false;
}

void SchedulerQueue::issue(ExecutionQueue *exQueue)
{
  int bw_left = bw;
  for( InsList::iterator it = queue.begin();
      (it!= queue.end())&&bw_left>0; it++)
  {
    if(rob[*it].ready())
    {
      if(exQueue->push(*it))
      {
        rob[*it].state(EX);
        rob[*it].timer = 0;
        *it = -1;
        bw_left--;
      }
      else
        break;
    }
  }
  queue.remove_if(invalid_index);
}

void SchedulerQueue::wake(int dest)
{
  for( InsList::iterator it = queue.begin();
      it!= queue.end(); it++)
  {
    rob[*it].update_src(dest);
  }
}

