#include "QUEUE.h"
#include "Schedule.h"
#include <iostream.h>

Queue::Queue() {
	head = 0;
	tail = 0;
	size = 0;
}

void Queue::addToQueue(PCB* newPcb, int sleepInterval) {
	QNode* temp = new QNode;
	temp->pcb = newPcb;
	temp->next = 0;
	temp->sleepInterval = sleepInterval;
	if (head == 0) {
		head = temp;
		tail = temp;
	}else{
		tail->next = temp;
		tail = temp;
	}
	size++;
}

PCB* Queue::getFromQueue() {
	if (head == 0)
		return 0;
	PCB* retPcb = head->pcb;
	QNode* temp = head;
	head = head->next;
	if (head == 0)
		tail = 0;
	delete (temp);
	size--;
	return retPcb;
}

void Queue::printQueue() const{
	QNode* temp = head;
	while (temp != 0) {
		cout << temp->pcb->uniqueID << " ";
		temp = temp->next;
	}
	cout << endl;
}

void Queue::removeFromQueue(PCB* toRemove){
    QNode *current = head;
    QNode *previous = 0;
    while(current != 0 && current->pcb != toRemove){
   	 previous = current;
   	 current = current->next;
    }
    if(current == tail->next)
    	return;
    if(previous == 0){
   	 head = current->next;
   	 delete current;
    }else if(current == tail){
   	 tail = previous;
   	 previous->next = 0;
    }else{
   	 previous->next = current->next;
   	 delete current;
    }
    size--;
}

int Queue::isEmpty(){
	return (size == 0);
}

int Queue::updateQ() {
	if (size == 0)
		return 0;
	QNode* focus = head, * after = 0;
	int counter = 0;
	while (focus != 0) {
		after = focus->next;
		if (focus->sleepInterval != -1) {
			focus->sleepInterval--;
			if (focus->sleepInterval == 0){
				counter++;
				//pravo budjenje niti
				focus->pcb->blockedFLAG = 0;
				focus->pcb->readyFLAG = 1;
				focus->pcb->semStatus = 0;
				Scheduler::put(focus->pcb);
				//kraj budjenja
				removeFromQueue(focus->pcb);
			}
		}
		focus = after;
	}
	return counter;
}

Queue::~Queue(){
	QNode* focus = head, * nextn = 0;
	while(focus != 0){
		nextn = focus->next;
		delete focus;
		focus = nextn;
	}
	head = 0;
	tail = 0;
	size = 0;
}
