#include <stdio.h>
#include <stdlib.h>
#include "ReadyQueue.h"

aPCB* makePCB(FILE *file)
{
    aPCB *newPCB = (aPCB *)malloc(sizeof(aPCB));
    newPCB->PC = file;
    return newPCB;
}
