#include "struct.h"
#include "cpu.c"
#include "ReadyQueue.h"

#ifndef interpreter_h
#define interpreter_h

int interpreter(char *words[], storage_t *memory, aCPU* core, readyQueue *rQ);
#endif /* interpreter */
