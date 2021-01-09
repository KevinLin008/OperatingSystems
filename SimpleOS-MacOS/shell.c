// NAME: Kevin Lin
// ID: 260704967

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shellmemory.h"
#include "interpreter.h"
#include "cpu.c"
#include "ReadyQueue.h"


int parse(char ui[], storage_t *memory, aCPU *core, readyQueue *rQ)
{
  int a,b;
  int w = 0;
  char tmp[200];
  char *words[50];

  //skip blank spaces
  for (a = 0; ui[a] == ' ' && a < 1000; a++);

  while (ui[a] != '\0' && a < 1000)
  {
    //don't account for '\0', ' ', or '\n'
    for (b = 0; ui[a] != '\0' && ui[a] != ' ' && ui[a] != '\n' && a < 1000; a++, b++)
      tmp[b] = ui[a];

    tmp[b] = '\0';
    words[w] = strdup(tmp);
    a++; w++;
  }
  return interpreter(words, memory, core, rQ);
}
