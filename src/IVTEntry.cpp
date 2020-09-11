#include <dos.h>

#include "IVTEntry.h"

//izmena u odnosu na proslu verziju:
//ivt je sada staticni clan klase IVTEntry

IVTEntry * IVTEntry::ivTable[256];


IVTEntry::IVTEntry(IVTNo numEntry_, interruptPointer ip){
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	//sacuvaj staru prekidnu rutinu
	oldRoutine = getvect(numEntry_);
#endif
	//myEvent mora da se poziva pri kreiranju objekta!!
	myEvent = 0;
	//postavi broj ulaza
	numEntry = numEntry_;
	//postavi ovu prekidnu rutinu u tabelu prekidnih rutina
	ivTable[numEntry] = this;
#ifndef BCC_BLOCK_IGNORE
	//postavi novu prekidnu rutinu
	setvect(numEntry, ip);
	asm sti;
#endif
}

//ovo se mora pozvati pri kreiranju objekta!!
void IVTEntry::setEvent(KernelEv* myEv){
	myEvent = myEv;
}

//ukoliko je potrebno, pozvati staru prekidnu rutinu
void IVTEntry::callOldRoutine(){
		oldRoutine();
}

//pozovi signal operaciju uvezanog KernelEv
void IVTEntry::signal(){
	myEvent->signal();
}

IVTEntry::~IVTEntry(){
	//obavezno lock unlock ovde
#ifndef BCC_BLOCK_IGNORE
	asm cli
	setvect(numEntry, oldRoutine);
	asm sti
#endif
}
