#include "cpu.c"
#include "ReadyQueue.h"

void myinit(FILE *file, storage_t *memory, aCPU *core, readyQueue *rQ);
int scheduler(aCPU *core, readyQueue *rQ, storage_t *memory);
