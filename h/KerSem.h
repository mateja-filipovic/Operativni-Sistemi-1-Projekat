#ifndef _KERSEM_H_
#define _KERSEM_H_

//#include "Thread.h"
#include "QUEUE.h"
//kernel implementacija semafora
class PCB;


class KernelSem{
public:

	KernelSem(int init = 1);
	~KernelSem();

	//operacije wait i signal sa modifikacijama
	int wait(unsigned int maxTimeToWait);
	int signal(int n = 0);

	//vraca trenutnu vrednost semafora
	int value() const;

	//pomocne niti za (de)blokiranje niti
	void block(unsigned int maxTimeToWait);
	void unblock();
	void updateSem();

	//vrednost semafora
	int val;
	//red blokiranih niti na ovom semaforu
	Queue* blockedQ;

};


#endif
