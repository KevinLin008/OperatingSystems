#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "pcb.h"

// Global data structures
//

PCB *head = NULL, *tail=NULL;

void printPCB() {
	PCB *temp = head;

	printf("Addr:");

	while(temp!=NULL) {
		printf(" %s",temp->pid);
		temp = temp->next;
	}

	printf("\n");
}

int victimCleared(PCB *p, int frameNo) {
	int i;

	for(i=0;i<10 && p->pageTable[i]!=frameNo;i++);

	if (i==10) return 0;

	p->pageTable[i] = -1;
	return 1;
}

void clearVictimFrame(int frameNo) {
	PCB *temp = head;

	while(temp!=NULL) {
		if (victimCleared(temp,frameNo)) 
			return;
		temp = temp->next;
	}
}

PCB *makePCB(FILE *p, char *pid, int maxPages) {
	PCB *pcb;
	int i;

	if (p == NULL) return NULL;

	pcb = (PCB *) malloc(sizeof(PCB));
	if (pcb == NULL) return NULL;

	pcb->PC        = p;
	pcb->pid       = strdup(pid);
	pcb->PC_page   = 0;
	pcb->PC_offset = 4;
	pcb->pages_max = maxPages;
	pcb->next      = NULL;
	for(i=0;i<10;i++) pcb->pageTable[i]=-1;
}

void addToReady(PCB *p) {
	p->next = NULL;

	if (head == NULL) {
		head = p;
		tail = p;
	} else {
		tail->next = p;
		tail = p;
	}
}

PCB *getHead() {
	return head;
}
PCB *getFromReady() {
	PCB *temp;

	if (head == NULL) return NULL;

	temp = head;
	head = head->next;
	temp->next = NULL;

	if (head == NULL) tail = NULL;

	return temp;
}

