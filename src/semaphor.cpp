#include "semaphor.h"
#include <iostream.h>

extern void lock();
extern void unlock();

Semaphore::Semaphore (int init){
	myImpl = new KernelSem(init);
}

int Semaphore::wait (Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
}

int Semaphore::val () const{
	return myImpl->value();
}  // Returns the current value of the semaphore

Semaphore::~Semaphore (){
	lock();
	delete myImpl;
	unlock();
}
