#include "SemList.h"
#include <iostream.h>

SemList::SemList(){
    head = 0;
    tail = 0;
    size = 0;
}

void SemList::addNode(KernelSem* newSem){
       SNode *temp = new SNode;
       temp->sem = newSem;
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

void SemList::remove(KernelSem* semToRemove){
     SNode *current = new SNode;
     SNode *previous = new SNode;
     previous = 0;
     current = head;
     while(current != 0 && current->sem != semToRemove){
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


void SemList::update(){
	SNode* focus = head;
	while(focus != 0){
		focus->sem->updateSem();
		focus = focus->next;
	}
}

SemList::~SemList(){
		SNode* focus = head, * nextn = 0;
		while(focus != 0){
			nextn = focus->next;
			delete focus;
			focus = nextn;
		}
		head = 0;
		tail = 0;
		size = 0;
}
