#include "PCB.h"
#include "IdleTh.h"

extern void inic();
extern void restore();
extern PCB* running;
extern Idle* idle;
extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){
	
  inic();
  running = new PCB();
  idle = new Idle(100, 1);
  int ret = userMain(argc, argv);
  restore();

  //delete idle;
  //delete allPCBs;
  //delete allSems;
  //delete running;
 
  return ret;
}

