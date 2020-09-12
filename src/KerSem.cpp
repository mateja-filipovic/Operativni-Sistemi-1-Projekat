#include "PCB.h"
#include "QUEUE.h"
#include "Schedule.h"
#include "KerSem.h"
#include "SemList.h"
#include <iostream.h>

extern void lock();
extern void unlock();
extern SemList* allSems;
extern PCB* running;

//izmena: umesto globalne liste uspavanih niti
//postoji globalna lista svih semafora koji se
//updatuju u prekidnoj rutini tajmera

KernelSem::KernelSem(int init){
	lock();
	//postavi inicijalnu vrednost semafora
	val = init;
	//napravi novi red
	blockedQ = new Queue();
	allSems->addNode(this);
	unlock();
}

int KernelSem::wait(unsigned int maxTimeToWait){

	lock();
	//ako je vrednost semafora 0 ili manje, blokiraj nit
	if(--val < 0){
		//cout << "nit se uspavala na ovoliko kvantova: " << maxTimeToWait << endl;
		block(maxTimeToWait);
	}
	unlock();
	//semStatus je flag u PCB strukturi, 0 oznacava da je oslobodjena zbog isteka vremena
	//1 oznacava da je oslobodio semafor
	return running->semStatus;
}

int KernelSem::signal(int n){

	lock();
	//ne sme se kao argument proslediti negativna vrednost
	if(n < 0)
		return n;
	//ako je n = 0, ova operacija se ponasa kao standardna, tj. n = 1
	if(n == 0)
		n = 1;
	int counter = 0;
	//deblokiraj onoliko niti koliko je potrebno/moguce
	while(n > 0 && val < 0){
		if(blockedQ->isEmpty())
			break; //dodato
		unblock();
		val++;
		n--;
		counter++;
	}
	//povecaj val za broj deblokiranih niti i vise ako je potrebno
	val += n ;//da li treba +counter??
	unlock();
	//vrati broj stvarno oslobodjenih niti
	return counter;

}

int KernelSem::value() const{
	return val;
}

//pomocna fja za blokiranje niti
void KernelSem::block(unsigned int maxTimeToWait){
	//postavi blockedFLAG na 1
	running->blockedFLAG = 1;
	//dodato
	running->readyFLAG = 0;
	running->semStatus = 1;
	//ako je neograniceno vreme cekanja, postavi FLAG za to na -1
	//u suprotnom, postavi zadato vreme cekanja
	int toWait = (maxTimeToWait == 0) ? -1 : maxTimeToWait;
	//dodaj ovu nit u red cekanja
	blockedQ->addToQueue(running, toWait);
	//running->blockedOn = this;
	//trazi promenu konteksta
	dispatch();
}

void KernelSem::unblock(){
	//provera da li uopste postoje blokirane niti
	//jer se moze desiti da su se deblokirale usled isteka vremena
	if(blockedQ->isEmpty()){
		//cout << "blocked queue empty" << endl;
		return;
	}
	//dohvati prvu nit iz reda i vrati je u rasporedjivac
	PCB* p = blockedQ->getFromQueue();
	p->blockedFLAG = 0;
	p->readyFLAG = 1;
	Scheduler::put(p);
}

//dodat metod za azuriranje stanja semafora
void KernelSem::updateSem(){
	val += blockedQ->updateQ();
}

KernelSem::~KernelSem(){
	//obrisi red i obrisi ovaj semafor iz globalne liste
	delete blockedQ;
	allSems->remove(this);
}

