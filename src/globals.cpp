#include "Schedule.h"
#include "PCB.h"
#include "List.h"
#include "SemList.h"
#include "Semaphor.h"
#include "IdleTh.h"
#include "user.h"

//lista svih PCBova
List* allPCBs = new List();
//pcb tekuce niti
PCB* running;
//lista svih semafora koji se azuriraju u prekidnoj rutini tajmera
SemList* allSems = new SemList();
//nit koja uposleno ceka
Idle* idle;


//promenljiva i fje za zabranu promene konteksta bez zabrane prekida
int lockFlag = 1;
void lock();
void unlock();
//pomocne promenljive za prekid tajmera
unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int brojac = 20;
volatile int zahtevana_promena_konteksta = 0;

void interrupt timer(){	// prekidna rutina
	if (!zahtevana_promena_konteksta){
		brojac--;
		if(allSems->size != 0)
			allSems->update();
	}
	if ((running->timeSlice != 0 && brojac == 0) || zahtevana_promena_konteksta) {
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		running->sp = tsp;
		running->ss = tss;
		running->bp = tbp;

		//running= getNextPCBToExecute();	// Scheduler
		if(running->readyFLAG && (running != idle->getPCB()))
			Scheduler::put(running);
		running = Scheduler::get();

		if(running == 0)
			running = idle->getPCB();

		tsp = running->sp;
		tss = running->ss;
		tbp = running->bp;

		brojac = running->timeSlice;

		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp
		}
	}

	// poziv stare prekidne rutine koja se
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta, tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida
if(!zahtevana_promena_konteksta) asm int 60h;

	zahtevana_promena_konteksta = 0;
}

void dispatch();

unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina

// postavlja novu prekidnu rutinu
void inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

// vraca staru prekidnu rutinu
void restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}



void lock(){
	lockFlag = 0;
}

void unlock(){
	lockFlag = 1;
	if(zahtevana_promena_konteksta)
		dispatch();
}

void dispatch(){
	asm cli;
	zahtevana_promena_konteksta = 1;
	timer();
	asm sti;
}
