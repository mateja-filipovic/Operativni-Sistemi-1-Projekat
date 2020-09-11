#ifndef _IDLETH_H_
#define _IDLETH_H_

#include "thread.h"

//nit koja uposleno ceka, ako su sve niti blokirane
class Idle: public Thread{
public:

	Idle(StackSize stackSize = 100, Time timeSlice = 2);
	/*virtual*/ void run();
};

#endif
