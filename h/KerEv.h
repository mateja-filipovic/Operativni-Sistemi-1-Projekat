#ifndef _KEREV_H_
#define _KEREV_H_

#include "PCB.h"

//kernel implementacija dogadjaja
class KernelEv{
public:

	//prilikom kreiranja mora se postaviti vlasnik i broj ulaza u IVT
	KernelEv(unsigned char ivtNo_);
	~KernelEv();

	void wait();
	void signal();


	//vlasnik objekta
	PCB* owner;
	//pokazivac na blokiranu nit, moze biti ili vlasnik ili 0
	PCB* blocked;
	//broj ulaza u IVT
	unsigned char ivtNo;
	//vrednost binarnog semafora
	int val;

};

#endif
