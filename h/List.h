#ifndef _LIST_H_
#define _LIST_H_

class PCB;

struct Node {
    PCB* pcb;
    Node *next;
 };


class List{
private:
  Node *head, *tail;
public:

  int size;

  List();
  ~List();

  void addNode(PCB* newPCB);
  void remove(PCB* pcbToRemove);
  PCB* findPCBById(int uniqueID);
  void wakeAll();
  int isEmpty();

};
#endif
