#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "struct.h"
#include "ram.h"
#include "pcb.h"
#include "ReadyQueue.h"
#include "cpu.c"
#include "interpreter.h"

void addToReady(aPCB* node_PCB, readyQueue *rQ)
{
  //first node
  if(rQ->head == NULL)
  {
    rQ->head = node_PCB;
    rQ->tail = node_PCB;
  }
  //append to tail
  else
  {
    aPCB *tempPCB = (aPCB *)malloc(sizeof(aPCB));
    tempPCB = rQ->tail;
    tempPCB->next = node_PCB;
    rQ->tail = node_PCB;
  }
}

int runQuanta(aCPU *core, readyQueue *rQ, storage_t *memory)
{
  int errCode;
  core->IP = rQ->head->PC;
  char *result = fgets(core->IR, sizeof core->IR, core->IP);
  //the case where EOF is reached
  if(result == NULL)
  {
    return 0;
  }
  //normal execution
  else
  {
    errCode = parse(core->IR, memory, core, rQ);
    //if it was a quit
    if(errCode == -1){exit(99);}
  }
  return 1;
}

int scheduler(aCPU *core, readyQueue *rQ, storage_t *memory)
{

  //main loop
  while(1)
  {
    //one task (2quanta) iteration
    while(core->quanta > 0)
    {
      //run one line of command
      int decision = runQuanta(core, rQ, memory);

      //the case where a file reaches EOF
      if(decision == 0)
      {
        //remove head PCB with new one
        aPCB* temp = rQ->head->next;
        freeRam(core->IP);
        fclose(rQ->head->PC);
        // free(rQ->head);
        rQ->head = temp;

        //if the next node is also gone then finish calls
        int cancel = runQuanta(core, rQ, memory);
        if(cancel == 0){return 0;}
      }
      //succeessful execution
      else{core->quanta--;}
    }

    //task switch
    rQ->head->PC = core->IP;

    aPCB *temp = (aPCB *)malloc(sizeof(aPCB));
    temp = rQ->head;
    addToReady(temp, rQ);
    rQ->head = rQ->head->next;
    core->quanta = 2;
  }
  return 0;
}

void myinit(FILE *file, storage_t *memory, aCPU *core, readyQueue *rQ)
{
  //make new PCB for FILE and add to ready queue
  aPCB *newPCB = makePCB(file);
  addToRAM(file);
  addToReady(newPCB, rQ);
}

int main()
{
  storage_t *head = (storage_t *)malloc(sizeof(storage_t));
  //to just initialize the first node, but not really using
  strcpy(head->variableName, "dummy");
  strcpy(head->valueStored, "dummy");

  //initialize CPU struct and ReadyQueue struct
  aCPU *core = (aCPU *)malloc(sizeof(aCPU));
  core->quanta = 2;
  readyQueue *rQ = (readyQueue *)malloc(sizeof(readyQueue));
  rQ->head = NULL;

  char command;
  int choice = 0;

  printf("Kernel 1.0 loaded!\n");
  printf("Welcome to the Kevin Lin shell!\n\rShell Version 2.0 Updated February 2019\n");

  char prompt[100] = {'$','\0'};
  char userInput[1000];
  int errorCode = 0;

  while(1)
  {
    printf("%s", prompt);
    fgets(userInput, 999, stdin);

    errorCode = parse(userInput, head, core, rQ);
    if (errorCode == -1)
    exit(99);
  }
}
