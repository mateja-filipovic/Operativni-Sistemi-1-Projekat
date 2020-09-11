#ifndef _SEMLIST_H_
#define _SEMLIST_H_

#include "KerSem.h"

struct SNode {
    KernelSem* sem;
    SNode *next;
 };


class SemList{
private:
  SNode *head, *tail;
public:

  int size;

  SemList();
  ~SemList();

  void addNode(KernelSem* newPCB);
  void remove(KernelSem* pcbToRemove);
  void update();

};
#endif
