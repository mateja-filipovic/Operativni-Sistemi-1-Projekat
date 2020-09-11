#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "PCB.h"

struct QNode {
	PCB* pcb;
	int sleepInterval;
	QNode* next;
};

class Queue {

public:

	Queue();
	~Queue();

	void addToQueue(PCB* newPcb, int sleepInterval);
	PCB* getFromQueue();
	void printQueue() const;
	void removeFromQueue(PCB* toRemove);
	int updateQ();
	int isEmpty();

//private:
	QNode* head, * tail;
	int size;
};
#endif
