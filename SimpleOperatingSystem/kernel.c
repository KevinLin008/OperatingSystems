#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "pcb.h"
#include "shell.h"
#include "ram.h"
#include "cpu.h"
#include "DISK_driver.h"

// return true or false
int myinit(PCB *pcb) {
	if (pcb != NULL) {
		addToReady(pcb);
		return 1;
	}

	return 0;
}

void terminate(PCB *p) {
	int i;
	char command[1000];

	// remove from ram
	for(i=0;i<10;i++) {
		if (p->pageTable[i] != -1)
			clearRAM(p->pageTable[i]);
	}

	// remove from backing store
	strcpy(command, "rm -f ");
	strcat(command, p->pid);
	system(command);

	// delete the PCB
	free(p);
}

void scheduler() {
	PCB *pcb;
	int result;

	// initialize CPU
	initCPU();

	// execute the processes
	while(getHead() != NULL) {
		// printPCB(); // for debugging

		pcb = getFromReady();

		if (pcb != NULL) {
			// task switch in
			setCPU(pcb->PC,pcb->PC_page,pcb->PC_offset);

			// run on CPU
			result = runCPU(2,pcb);

			// task switch out
			pcb->PC        = getCPUpc();
			pcb->PC_page   = getCPUpage();
			pcb->PC_offset = getCPUoffset();

			// terminate or add to ready
			if (result == 99) terminate(pcb);
			else addToReady(pcb);
		}
	}
}

void boot() {
	// Initialize RAM
	initRAM();

	// Initialize the backing store
	system("mkdir BackingStore");
	system("rm -f BackingStore/*");

	// Initialize the CPU
	initCPU();
}

int main() {
	int result = 0;

	boot();

	result = shell();

	return result;
}
