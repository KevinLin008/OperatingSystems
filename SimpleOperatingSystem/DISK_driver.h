#ifndef CPUH
#define CPUH
void initIO();
int partitionMethod(char *name, int blocksize, int totalblocks);
int mount(char *name);
int searchFP();
int assignBlock(FILE *file, int index);
int openfile(char *name);
int readBlock(int file);
int writeBlock(int file, char *data);
#endif
