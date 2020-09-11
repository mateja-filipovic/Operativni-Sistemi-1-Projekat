#include "IdleTh.h"
#include <iostream.h>

//konstruktor korisnicke niti
Idle::Idle(StackSize stackSize, Time timeSlice) : Thread(stackSize, timeSlice){}

//zadatak ove niti je da uposleno ceka, ako su sve niti blokirane
void Idle::run(){
	while(1){
		dispatch();
	}
}
