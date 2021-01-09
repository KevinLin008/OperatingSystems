#include <stdio.h>

#include "memorymanager.h"
#include "shell.h"
#include "pcb.h"

// Global data structures simulating hardware
//

struct CPU {
	FILE *IP;
	char IR[1000];
	int quanta;
	int PC_page;   // the page we are pointing to
	int PC_offset; // 0 to 3, at 4 a page fault occurs
} cpu;

void initCPU() {
	cpu.quanta     = 0;
	cpu.IP         = NULL;
	cpu.IR[0]      = '\0';
	cpu.PC_page    = -1;
	cpu.PC_offset  = 0;
}

void setCPU(FILE *PC, int pageNo, int offset) {
	cpu.IP        = PC;
	cpu.IR[0]     = '\0';
	cpu.PC_page   = pageNo;
	cpu.PC_offset = offset;
}

FILE *getCPUpc() { return cpu.IP; }

int getCPUpage() { return cpu.PC_page; }

int getCPUoffset() { return cpu.PC_offset; }

int pageFault(PCB *pcb) {
	int nextFrame;
	int result;
	FILE *temp;

	// set veriables to get next page
	cpu.PC_page++;
	cpu.PC_offset = 0;

	// check to see if there is another page
	if (cpu.PC_page < 10) {
		nextFrame = pcb->pageTable[cpu.PC_page];

		if (nextFrame != -1) return 0; // page already loaded
		if (nextFrame == -1 && cpu.PC_page >= pcb->pages_max) 
			return 2; // end of program
		else if (nextFrame == -1) { // load next page
			temp = findPage(cpu.PC_page, cpu.IP);
			if (temp == NULL) return 3; // could not load
			result = findFrame(temp);
			if (result == -1) return 3; // could not load
			pcb->pageTable[cpu.PC_page] = result;
			cpu.IP = temp;
		} else return 4; // unexpected error
	}
	else
		return 1; // error, no more pages

	return 0; // loaded successfully
}

int runCPU(int quanta, PCB *pcb) {
	int result;
	char *p;

	cpu.quanta = quanta;

	while(cpu.quanta > 0 && cpu.PC_offset < 4) {
		p = fgets(cpu.IR, 999, cpu.IP);

		if (p == NULL || feof(cpu.IP)) return 99; // end of program

		// printf(">>>%s\n",cpu.IR); // debug code

		result = prompt(cpu.IR);
		if (result != 0) return result;

		cpu.quanta--;
		cpu.PC_offset++;
	}	

	if (cpu.PC_offset >= 4) pageFault(pcb); // page fault

	return 0; // no errors
}
