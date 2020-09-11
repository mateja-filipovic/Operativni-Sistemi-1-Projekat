#include "KerEv.h"
#include "Schedule.h"
#include "IVTEntry.h"

extern void dispatch();
extern PCB* running;
extern void lock();
extern void unlock();

//izmena: vlasnik se ne postavlja preko konstruktora
//vec se implicitno u konstruktoru uzima running

KernelEv::KernelEv(unsigned char ivtNo_){
	owner = running;
	blocked = 0;
	ivtNo = ivtNo_;
	//inicijalno je vrednost 0
	val = 0;
	//uvezati IVTEntry objekat sa ovim objektom
	IVTEntry::ivTable[ivtNo_]->setEvent(this);
}

//izmenjen je nacin rada wait i signal
void KernelEv::wait(){
	lock();
	if(running != owner){
		unlock();
		return;
	}
	if(val == 0){
		//val = 1;
		owner->blockedFLAG = 1;
		owner->readyFLAG = 0;
		blocked = owner;
		dispatch();
	}else{
		val = 0;
	}
	unlock();
}

void KernelEv::signal(){
	lock();
	if(blocked == 0)
		val = 1;
	else{
		val = 0;
		blocked = 0;
		owner->readyFLAG = 1;
		owner->blockedFLAG = 0;
		Scheduler::put(owner);
	}
	unlock();
}

KernelEv::~KernelEv(){
	//da li treba ovo dodavati??
	//IVTEntry::ivTable[ivtNo]->myEvent = 0;
}
