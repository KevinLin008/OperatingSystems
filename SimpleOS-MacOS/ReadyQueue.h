#ifndef ReadyQueue_h
#define ReadyQueue_h

typedef struct PCB
{
  FILE *PC;
  struct PCB *next;
}aPCB;

typedef struct readyQueue
{
  struct PCB *head;
  struct PCB *tail;

}readyQueue;

#endif /* ReadyQueue_h */
