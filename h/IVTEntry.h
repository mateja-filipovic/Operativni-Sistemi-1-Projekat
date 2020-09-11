#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

#include "PCB.h"
#include "KerEv.h"

typedef unsigned char IVTNo;
typedef void interrupt(*interruptPointer)(...);

class IVTEntry{
public:

	IVTEntry(IVTNo numEntry, interruptPointer ip);
	~IVTEntry();


	//postavljanje eventa, klase IVTEntry i KernelEv su uvezane 1 na 1
	void setEvent(KernelEv* myEv);
	//pozivanje stare prekidne rutine, ukoliko je to navedeno pri stvaranju objekta
	void callOldRoutine();
	//poziva signal na KernelEv
	void signal();

	//stara prekidna rutina
	interruptPointer oldRoutine;
	//broj ulaza u IVT
	IVTNo numEntry;
	//implementacija Eventa od strane jezgra
	KernelEv* myEvent;
	//tabela prekidnih rutina
	static IVTEntry* ivTable[];
};


#endif
