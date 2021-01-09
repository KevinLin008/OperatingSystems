

// char **createQueue()
// {
//   char **queue = malloc(10 * sizeof(char*));
//   for (int i = 0; i < 10; i++)
//   {
//     queue[i] = malloc(100 * sizeof(char));
//   }
//   return queue;
// }

/*
The argument cmd = 0 for a read and 1 for a write.
The argument data contains the information to write.
The function IOscheduler() returns the data read.
The argument ptr tracks who this is for
*/
#include<stdlib.h>
#include<stdio.h>
#include "interpreter.h"

struct circularArray
{
  char *commandLine[100];
  char *data;
  PCB *ptr;
  int cmd;
};

struct circularArray *queue[10];

char *IOscheduler(char *data, PCB *ptr, int cmd)
{
  

}
