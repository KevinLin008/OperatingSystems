// NAME: Kevin Lin
// ID: 260704967

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"

int add(char variable[], char value[], storage_t *memory)
{
  storage_t *newTail = malloc(sizeof(storage_t));

  //to go through the structure and find the appropriate place
  while(memory != NULL){
    //in the case where the list is empty
    if(strcmp(memory->variableName, "dummy") == 0)
    {
      strcpy(memory->variableName, variable);
      strcpy(memory->valueStored, value);
      memory->next = newTail;
      newTail = NULL;
      return 0;
    }
    //in the case where the list is non-empty
    else
    {
      if(memory->next == NULL)
      {
        storage_t *tmp = malloc(sizeof(storage_t));
        strcpy(tmp->variableName, variable);
        strcpy(tmp->valueStored,value);
        tmp->next = NULL;

        newTail = tmp;
        memory->next = newTail;
        return 0;
      }
    }
    memory = memory->next;
  }
  return 0;
}

//if variableName found return 0 o/w 1
int check(char variable[], storage_t *memory)
{
  storage_t *ptr = (storage_t *)malloc(sizeof(storage_t));
  ptr = memory;

  while(ptr != NULL)
  {
    //in the case where the variableName already exists
    if(strncmp(ptr->variableName, variable, 50) == 0)
    {
      return 0;
    }
    ptr = ptr->next;
  }

  return 1;
}

//displays the valueStored at the given variableName
int display(char variable[], storage_t *memory)
{
  storage_t *ptr = malloc(sizeof(storage_t));
  ptr = memory;

  while(ptr != NULL)
  {
    //print the valueStored of the passed variableName
    if(strcmp(ptr->variableName, variable) == 0)
    {
      printf("%s\n", ptr->valueStored);
      return 0;
    }
    ptr = ptr->next;
  }
  return 0;
}

//if the variable already exists overwrite its valueStored
void replace(char variable[], char replacement[], storage_t *memory)
{
  storage_t *ptr = malloc(sizeof(storage_t));
  ptr = memory;

  while(ptr != NULL)
  {
    if(strcmp(ptr->variableName, variable) == 0)
    {
      strcpy(ptr->valueStored, replacement);
    }
    ptr = ptr->next;
  }
}
