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


void print_config(int N, int S, DispatchQueue *dQ, int cycles);
void print_stats(ReorderBuffer &rob);

int main(int argc, char **argv)
{
  if( argc < 4 )
  {
    cout<<"invalid number of arguments"<<endl;
    cout<<"sim <S> <N> <BLOCK_SIZE> <L1_SIZE> <L1_ASSOC> <L2_SIZE> <L2_ASSOC> <TRACE_FILE>"<<endl;
    return -1;
  }
  int superscale_degree   = atoi( argv[1] );
  int scheduler_size      = atoi( argv[2] );
  char * filename         = argv[3];

  RegisterFile rfile;
  ReorderBuffer rob;
  TraceReader tr = TraceReader(filename);

  DispatchQueue  *dQ = new DispatchQueue(superscale_degree, rob, tr);
  SchedulerQueue *sQ = new SchedulerQueue(scheduler_size, superscale_degree, rob, rfile);
  ExecutionQueue *eQ = new ExecutionQueue(superscale_degree, rob, rfile);
  do
  {
    rob.retire();
    eQ->execute(sQ);
    sQ->issue(eQ);
    dQ->dispatch(sQ);
    dQ->fetch();
  //  tr++;
    Instruction::tick();
  }while(rob);
  rob.prepare_result();
  print_stats(rob);
  print_config(superscale_degree,scheduler_size,dQ,Instruction::cycles()-1);
  return 0;
}

void print_stats(ReorderBuffer &rob)
{
  for(int i=0; i<rob.size(); i++)
  {
    printf("%d fu{%d} ",i,rob[i].op);
    printf("src{%d,%d} dst{%d} ",rob[i].s1,rob[i].s2,rob[i].d);
    printf("IF{%d,%d} ",rob[i][IF].start,rob[i][IF].end-rob[i][IF].start);
    printf("ID{%d,%d} ",rob[i][ID].start,rob[i][ID].end-rob[i][ID].start);
    printf("IS{%d,%d} ",rob[i][IS].start,rob[i][IS].end-rob[i][IS].start);
    printf("EX{%d,%d} ",rob[i][EX].start,rob[i][EX].end-rob[i][EX].start);
    printf("WB{%d,%d}\n",rob[i][WB].start,rob[i][WB].end-rob[i][WB].start);
  }
}

void print_config(int N, int S, DispatchQueue *dQ, int cycles)
{
  printf("CONFIGURATION\n");
  printf("superscalar bandwidth (N) = %d\n",N);
  printf("dispatch queue size (2*N) = %d\n",2*N);
  printf("schedule queue size (S)   = %d\n",S);
  printf("RESULTS\n");
  printf("number of instructions = %d\n",dQ->num_instructions());
  printf("number of cycles       = %d\n",cycles);
  printf("IPC                    = %1.2f\n",(dQ->num_instructions()/(double)cycles));
}
