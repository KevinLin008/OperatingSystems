// NAME: Kevin Lin
// ID: 260704967

#include <stdio.h>
#include <string.h>
#include "shellmemory.h"
#include "shell.h"
#include "kernel.h"
#include "cpu.c"
#include "ReadyQueue.h"

int script(char *words[], storage_t *memory, aCPU* core, readyQueue *rQ)
{
  int errCode = 0;
  FILE *file = fopen(words[1], "rt");
  if(file != NULL)
  {
    char line[1000];
    while(fgets(line, sizeof line, file) != NULL)
    {
      errCode = parse(line, memory, core, rQ);
    }
    fclose(file);
  }
  //if the file didn't open
  else
  {
    printf("Script not found\n");
  }
  return errCode;
}

int help(char *words[])
{
  printf("These are the possible commands:\nset\nprint\nrun\nquit\nexec 'file1' 'file2' up to 'file3'\n");
  return 0;
}

int set(char *words[], storage_t *memory)
{
  //the case where variableName DNE
  if(check(words[1], memory) == 1)
  {
    add(words[1], words[2], memory);
  }
  //the case where variableName exists
  if(check(words[1], memory) == 0)
  {
    replace(words[1], words[2], memory);
  }
  return 0;
}

int print(char *words[], storage_t *memory)
{
  if(check(words[1], memory) == 0)
  {
    display(words[1], memory);
  }
  else
  {
    printf("The variable does not exist\n");
  }
  return 0;
}

int run(char *words[], storage_t *memory, aCPU *core, readyQueue *rQ)
{
  int errCode = 0;
  errCode = script(words, memory, core, rQ);
  return errCode;
}

int exec(char *words[], storage_t *memory, aCPU *core, readyQueue *rQ)
{
  int errCode = 0;
  int count;

  //see how many files are inputed
  if(words[1] == NULL)
  {
    printf("Input a valid filename\n");
    return 0;
  }
  else if(words[2] == NULL) count = 1;
  else if(words[3] == NULL) count = 2;
  else if(words[4] == NULL) count = 3;

  //search for repeated scripts
  if(count == 1)
  {
    FILE *file = fopen(words[1], "rt");
    if(file != NULL)
    {
      //initialize PCB's with file pointers and readyQueue with PCBs
      myinit(file, memory, core, rQ);
    }
    else
    {
      printf("Error opening file, %s\n", words[1]);
      return errCode;
    }
  }
  else if(count == 2)
  {
    if(strcmp(words[1], words[2]) == 0)
    {
      printf("Error: Script %s already loaded\n", words[2]);
      FILE *file = fopen(words[1], "rt");
      if(file != NULL)
      {
        //initialize PCB's with file pointers and readyQueue with PCBs
        myinit(file, memory, core, rQ);
      }
      else
      {
        printf("Error opening file, %s\n", words[1]);
        return errCode;
      }
    }
    else
    {
      for(int i = 1; i <= 2; i++)
      {
        FILE *file = fopen(words[i], "rt");
        if(file != NULL)
        {
          //initialize PCB's with file pointers and readyQueue with PCBs
          myinit(file, memory, core, rQ);
        }
        else
        {
          printf("Error opening file, %s\n", words[i]);
          return errCode;
        }
      }

    }
  }
  else if(count == 3)
  {
    if(strcmp(words[1], words[2]) == 0)
    {
      printf("Error: Script %s already loaded\n", words[2]);
      FILE *file = fopen(words[1], "rt");
      if(file != NULL)
      {
        //initialize PCB's with file pointers and readyQueue with PCBs
        myinit(file, memory, core, rQ);
      }
      else
      {
        printf("Error opening file, %s\n", words[1]);
        return errCode;
      }
    }
    if(strcmp(words[1], words[3]) == 0)
    {
      printf("Error: Script %s already loaded\n", words[3]);
      FILE *file = fopen(words[1], "rt");
      if(file != NULL)
      {
        //initialize PCB's with file pointers and readyQueue with PCBs
        myinit(file, memory, core, rQ);
      }
      else
      {
        printf("Error opening file, %s\n", words[1]);
        return errCode;
      }
    }
    if(strcmp(words[2], words[3]) == 0)
    {
      printf("Error: Script %s already loaded\n", words[3]);
      FILE *file = fopen(words[2], "rt");
      if(file != NULL)
      {
        //initialize PCB's with file pointers and readyQueue with PCBs
        myinit(file, memory, core, rQ);
      }
      else
      {
        printf("Error opening file, %s\n", words[2]);
        return errCode;
      }
    }
    else
    {
      for(int i = 1; i <= 3; i++)
      {
        FILE *file = fopen(words[i], "rt");
        if(file != NULL)
        {
          //initialize PCB's with file pointers and readyQueue with PCBs
          myinit(file, memory, core, rQ);
        }
        else
        {
          printf("Error opening file, %s\n", words[i]);
          return errCode;
        }
      }
    }
  }

  //  int i = 1;
  //
  //  while(1)
  //  {
  //    if(words[i] != NULL)
  //    {
  //      FILE *file = fopen(words[i], "rt");
  //      if(file != NULL)
  //      {
  //        //initialize PCB's with file pointers and readyQueue with PCBs
  //        myinit(file, memory, core, rQ);
  //        i++;
  //      }
  //      else
  //      {
  //        printf("Error opening file, %s\n", words[i]);
  //        return errCode;
  //      }
  //    }
  //    else{break;}
  //  }

  //call main loop
  errCode = scheduler(core, rQ, memory);
  return errCode;
}

int quit(char *words[])
{
  printf("Bye!\n");
  return -1;
}

int interpreter(char *words[], storage_t *memory, aCPU* core, readyQueue *rQ)
{
  int errCode = 0;

  if (strncmp(words[0], "help", 4) == 0)          errCode = help(words);
  else if (strncmp(words[0], "set", 3) == 0)      errCode = set(words, memory);
  else if (strncmp(words[0], "print", 5) == 0)    errCode = print(words, memory);
  else if (strncmp(words[0], "run", 3) == 0)      errCode = run(words, memory, core, rQ);
  else if (strncmp(words[0], "quit", 4) == 0)     errCode = quit(words);
  else if (strncmp(words[0], "exec", 4) == 0)   errCode = exec(words, memory, core, rQ);
  else
  {
    printf("Unknown command\n");
    // printf("%s\n", words[0]);
    return 0;
  }
  return errCode;
}
