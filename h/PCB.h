#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "List.h"

class KernelSem;


class PCB{
public:
	unsigned stackSize;
	unsigned* stack;
	unsigned sp, bp, ss;

	unsigned newFLAG, readyFLAG, blockedFLAG, finishedFLAG;

	Time timeSlice;
	Thread* myThread;
	List* waitingList;

	int uniqueID;
	static int nextID;

	int semStatus;

	PCB(StackSize stSz = defaultStackSize, Time tSl = defaultTimeSlice, Thread* myTh = 0);
	static void wrapper();
	void wakeUp();
	~PCB();
};

#endif
