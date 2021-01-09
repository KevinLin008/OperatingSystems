#include <stdio.h>

#ifndef CPU_h
#define CPU_h

typedef struct CPU
{
  FILE *IP;
  char IR[1000];
  //max is 2 quanta which stands for two lines of code
  int quanta;
}aCPU;

#endif /* CPU_h */
