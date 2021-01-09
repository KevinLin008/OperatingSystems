#ifndef RAMH
#define RAMH
// Public functions
//

void initRAM();
int addToRAM(FILE *p); // return startAddress or error codes
void clearRAM(int startAddressRAM);
int addToFrameNo(int frameNo, FILE *p);
#endif
