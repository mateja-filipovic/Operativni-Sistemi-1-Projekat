#include "thread.h"
#include "PCB.h"
#include "Schedule.h"
#include "List.h"
#include <iostream.h>


extern PCB* running;
extern List* allPCBs;
extern void lock();
extern void unlock();


Thread::Thread (StackSize stackSize, Time timeSlice){
	lock();
	myPCB = new PCB(stackSize, timeSlice, this);
	unlock();
}

//pokretanje niti
void Thread::start(){
	lock();
	if(myPCB->newFLAG != 1){
		unlock();
		return;
	}
	myPCB->newFLAG = 0;
	Scheduler::put(myPCB);
	unlock();
}

PCB* Thread::getPCB(){
	return myPCB;
}

void Thread::waitToComplete(){
	if(running->uniqueID == myPCB->uniqueID)
		return;
	if(myPCB->finishedFLAG)
		return;
	lock();
	running->blockedFLAG = 1;
	running->readyFLAG = 0;
	myPCB->waitingList->addNode(running);
	unlock();
	dispatch();
}

ID Thread::getId(){
	return myPCB->uniqueID;
}

ID Thread::getRunningId(){
	return running->uniqueID;
}

Thread* Thread::getThreadById(ID id){
	return allPCBs->findPCBById(id)->myThread;
}

Thread::~Thread(){
	waitToComplete();
	lock();
	delete myPCB;
	unlock();
}

