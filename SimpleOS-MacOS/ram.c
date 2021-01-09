#include <stdio.h>

FILE *ram[10];

int availableSpot()
{
    //iterate till the first NULL spot is found
    for(int i = 0; i < 10; i++)
    {
        if(ram[i] == NULL)
        {
            return i;
        }
    }
    return -1;
}

void addToRAM(FILE *file)
{
    int index = availableSpot();
    if(index != -1)
    {
      ram[index] = file;
    }
    else
    {
      printf("error loading into RAM");
      return;
    }
}

void freeRam(FILE *file)
{
  for(int i = 0; i < 10; i++)
  {
    if(ram[i] == file)
    {
      ram[i] = NULL;
      return;
    }
  }
}
