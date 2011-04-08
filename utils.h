#include <stdio.h>
#include "struct.h"
#include <stdlib.h> //for random
#include <string.h>
#include <time.h>

void clearRRam(rmdata *machine, unsigned char x);
void clearVRam(vmdata *machine, unsigned char x);
void clearRReg(rmdata *machine, unsigned char x);
void clearVReg(vmdata *machine, unsigned char x);
char fileToRam(rmdata *machine, char *file);
/*
Allocates random blocks of memory and creates a paging system
if success returns 1
else if not enough memory returns 0
*/
char allocateRam(rmdata *machine);
/*
Turns virtual offset into absolute(real machine)
using the paging system and the PTR register
*/
int absoluteAdress(rmdata *machine, int virtualAdress);

/*
Updates virtual machine from real machine using the paging system
*/
void updateMachine(rmdata *rmachine, vmdata *vmachine);