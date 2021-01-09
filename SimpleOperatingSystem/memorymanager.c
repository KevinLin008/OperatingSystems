#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ram.h"
#include "pcb.h"
#include "kernel.h"

// next PID number
int nextPIDNumber = 0;

// Assumes page count starts from 1
int countTotalPages(FILE *aFile) {
	char buffer[1000];
	int count = 0;

	// make sure at beginning of file
	rewind(aFile);

	// count the number of lines on file
	fgets(buffer,999,aFile);
	while(!feof(aFile)) {
		count++;
		fgets(buffer,999,aFile);
	}

	// if empty file return -1
	if (count == 0) return -1;

	// compute the number of pages
	return (count / 4) + 1;
}

// Assumes pageNumber starts at zero
FILE *findPage(int pageNumber, FILE *aFile) {
	char buffer[1000];
	int count = 0;
	int pageLineNumber = pageNumber * 4;

	// make sure at beginning of file
	rewind(aFile);

	// find the page
	do {
		if (count == pageLineNumber)
			return aFile;

		fgets(buffer,999,aFile);

		count++;
	} while(!feof(aFile));

	return NULL;
}

int findFrame(FILE *assignFrame) {
	int frameNumber = addToRAM(assignFrame);

	if (frameNumber < 0) return -1;

	return frameNumber;
}

int findVictim(PCB *p) {
	int start = rand()%10; // ten frames in RAM
	int r = start;
	int i;

	// find a page not from PCB
	do {
		for(i=0;i<10 && p->pageTable[i]!=r;i++);
		// page found?
		if (i==10) return r;

		// go to next frame
		r = (r+1) % 10;
	} while(r != start);

	// all frames belong to PCB !!
	return -1;
}

int updateFrame(int frameNumber, int victimFrame, FILE *page) {
	int result;

	if (frameNumber == -1) result = addToFrameNo(victimFrame,page);
	else result = addToFrameNo(frameNumber, page);

	return result;
}

int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame) {
	if (frameNumber == -1) {
		p->pageTable[pageNumber]=victimFrame;
		clearVictimFrame(victimFrame);
	} else {
		p->pageTable[pageNumber]=frameNumber;
	}

	return 0;
}

// returns true or false
int launcher(FILE *p) {
	char buffer[1000];
	FILE *backPtr;
	char backName[100] = {'P','\0'};
	char filename[1000];
	int i,pid, frameNo, victimNo;
	int totalPages;

	// create the backing store file name
	
	pid = nextPIDNumber;
	nextPIDNumber++;

	if (pid==0) {
		backName[1] = '0';
		backName[2] = '\0';
	}
	else {
		int remainder = pid % 10;
		int number    = pid / 10;
		int i         = 1;

		do {
			backName[i] = remainder+48;
			i++;

			remainder = number % 10;
			number = number / 10;
		} while(number>0);

		backName[i] = '\0';
	}

	strcpy(filename,"BackingStore/");
	strcat(filename,backName);

	// copy file p to backing store
	backPtr = fopen(filename,"wt");
	if (backPtr == NULL) return -1;

	fgets(buffer,999,p);
	while(!feof(p)) {
		fputs(buffer,backPtr);
		fgets(buffer,999,p);
	}

	fclose(p);
	fclose(backPtr);

	backPtr = fopen(filename,"rt");
	if (backPtr == NULL) return -1;

	// create a new PCB for this process
	totalPages  = countTotalPages(backPtr); 
	PCB *newPCB = makePCB(backPtr,filename,totalPages);

	// load two pages into RAM
	
	if (totalPages == 0) return -1;

	if (totalPages > 2) totalPages = 2;

	for(i=0;i<totalPages;i++) {
		p        = findPage(i,backPtr);
		frameNo  = findFrame(p);
		victimNo = -1;

		if (frameNo == -1)
			victimNo = findVictim(newPCB);
		if (updateFrame(frameNo,victimNo,p)==0) updatePageTable(newPCB, i, frameNo, victimNo);
		else return 0;
	}

	// add PCB into read queue
	
	myinit(newPCB);

	return 1;
}
