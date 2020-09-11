#include "List.h"
#include "PCB.h"
#include "schedule.h"
#include <iostream.h>

#define lock asm cli
#define unlock asm sti

List::List(){
    head = 0;
    tail = 0;
    size = 0;
}

void List::addNode(PCB* newPCB){
       Node *temp = new Node;
       temp->pcb = newPCB;
       temp->next = 0;
       if(head == 0) {
         head = temp;
         tail = temp;
         temp = 0;
       }else {
         tail->next = temp;
         tail = temp;
       }
       size++;
  }

void List::remove(PCB* pcbToRemove){
     Node *current = new Node;
     Node *previous = new Node;
     previous = 0;
     current = head;
     while(current != 0 && current->pcb != pcbToRemove){
    	 previous = current;
    	 current = current->next;
     }
     if(current == tail->next) return;
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

PCB* List::findPCBById(int uniqueID){
	Node* temp = head;
	while(temp->pcb->uniqueID != uniqueID && temp != 0)
		temp = temp->next;
	if(temp == 0)
		cout << "Greska: PCB nije pronadjen pomocu PCBAT fje" << endl;
	return temp->pcb;
}


int List::isEmpty(){
	return (size == 0);
}


void List::wakeAll(){
	Node* focus = head, * nextNode = 0;
	while(focus != 0){
		nextNode = focus->next;
		focus->pcb->wakeUp();
		delete focus;
		focus = nextNode;
	}
}

List::~List(){
	Node* focus = head, * nextn = 0;
	while(focus != 0){
		nextn = focus->next;
		delete focus;
		focus = nextn;
	}
	head = 0;
	tail = 0;
	size = 0;
}
