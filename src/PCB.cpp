#include "PCB.h"
#include "Schedule.h"
#include <DOS.h>
#include <iostream.h>

extern void dispatch();
extern PCB* running;
extern List* allPCBs;
extern void lock();
extern void unlock();

int PCB::nextID = 0;

//izmena: umesto dva konstruktora koristi se jedan
//sa nekim default parametrima

PCB::PCB(StackSize stSz, Time tSl, Thread* myTh){
	uniqueID = nextID++;
	if(stSz >= 64000)
		stackSize = 64000;
	else
		stackSize = stSz / 2;

	//postavi nit i timeslice
	myThread = myTh;
	timeSlice = tSl;

	//inicijalizacija liste cekanja
	waitingList = new List();

	//inicijalna postavka flegova
	newFLAG = 1;
	readyFLAG = 1;
	blockedFLAG = 0;
	finishedFLAG = 0;

	//blockedOn = 0;
	semStatus = 0;

	//ne praviti kontekst za glavnu nit
	if(myTh != 0){
		stack = new unsigned[stackSize];
		stack[stackSize - 1] = 0x200;
		stack[stackSize - 2] = FP_SEG(PCB::wrapper);
		stack[stackSize - 3] = FP_OFF(PCB::wrapper);
		sp = FP_OFF(stack + stackSize - 12);
		ss = FP_SEG(stack + stackSize - 12);
		bp = sp;
	}else{
		stack = 0;
		sp = 0;
		bp = 0;
		ss = 0;
	}
	//registruj ovaj pcb u globalnu listu svih pcbova
	allPCBs->addNode(this);
}

void PCB::wrapper(){
	running->myThread->run();
	lock();
	running->waitingList->wakeAll();
	running->readyFLAG = 0;
	running->finishedFLAG = 1;
	unlock();
	dispatch();
}

void PCB::wakeUp(){
	blockedFLAG = 0;
	readyFLAG = 1;
	Scheduler::put(this);
}

PCB::~PCB(){
	allPCBs->remove(this);
	if(stack != 0)
		delete[] stack;
	delete waitingList;
}
