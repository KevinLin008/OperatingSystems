#ifndef CPUH
#define CPUH
void initCPU();
void setCPU(FILE *PC, int pageNo, int offset);
int runCPU(int quanta, PCB *pcb);
FILE *getCPUpc();
int getCPUpage();
int getCPUoffset();
#endif
