#ifndef MY_STORAGE
#define MY_STORAGE

typedef struct storage
{
  char variableName[50];
  char valueStored[50];
  struct storage *next;
} storage_t;

#endif /* MY_STORAGE */
