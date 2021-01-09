#ifndef PCBH
#define PCBH
typedef struct PCB_REC {
	FILE *PC;
	char *pid; // the process ID number
	int pageTable[10];
	int PC_page, PC_offset, pages_max;
	struct PCB_REC *next;
} PCB;

void addToReady(PCB *p);
PCB *getFromReady();
PCB *makePCB(FILE *p, char *pid, int maxPages);
PCB *getHead();
void printPCB();
void clearVictimFrame(int frameNo);
#endif
