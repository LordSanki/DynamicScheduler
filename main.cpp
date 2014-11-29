#include <cstring>
#include <cstdlib>
#include <iostream>
#include <Instruction.h>
#include <RegisterFile.h>
#include <TraceReader.h>
#include <DispatchQueue.h>
#include <SchedulerQueue.h>
#include <ExecutionQueue.h>

using namespace std;

int main(int argc, char **argv)
{
  if( argc < 4 )
  {
    cout<<"invalid number of arguments"<<endl;
    cout<<"sim <S> <N> <BLOCK_SIZE> <L1_SIZE> <L1_ASSOC> <L2_SIZE> <L2_ASSOC> <TRACE_FILE>"<<endl;
    return -1;
  }
  int scheduler_size      = atoi( argv[1] );
  int superscale_degree   = atoi( argv[2] );
  char * filename         = argv[3];

  RegisterFile rfile;
  ReorderBuffer rob;
  TraceReader tr = TraceReader(filename);

  DispatchQueue  *dQ = new DispatchQueue(superscale_degree, rob, tr);
  SchedulerQueue *sQ = new SchedulerQueue(scheduler_size, superscale_degree, rob, rfile);
  ExecutionQueue *eQ = new ExecutionQueue(superscale_degree, rob, rfile);
  do {
    rob.retire();
    eQ->execute(sQ);
    sQ->issue(eQ);
    dQ->dispatch(sQ);
    dQ->fetch();
    tr++;
  }while(rob);

  return 0;
}

