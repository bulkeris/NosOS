#include "utils.h"

void clearRRam(rmdata *machine, unsigned char x)
{
	int i;
	for(i = 0; i < 256; i++)
	{
		machine->ram[i] = x;
	}
}

void clearVRam(vmdata *machine, unsigned char x)
{
	int i;
	for(i = 0; i < 64; i++)
	{
		machine->ram[i] = x;
	}
}

void clearRReg(rmdata *machine, unsigned char x)
{
	machine->AX = x;
	machine->BX = x;
	machine->IP = x;
	machine->SF = x;
	machine->PTR = x;
	machine->PI = x;
	machine->SI = x;
	machine->TI = x;
	machine->MODE = x;
	machine->CN1 = x;
	machine->CN2 = x;
	machine->CN3 = x;
	machine->IOI = x;
}

void clearVReg(vmdata *machine, unsigned char x)
{
	machine->AX = x;
	machine->BX = x;
	machine->IP = x;
	machine->SF = x;
}

int atoh(unsigned char *c)
{
	int ret = 0;

	if(*c >= '0' && *c <= '9')
		ret = *c - '0';
	else if(*c >= 'a' && *c <= 'f')
		ret = *c - 'a' + 10;
	else if (*c >= 'A' && *c <= 'F')
		ret = *c - 'A' + 10;

    return ret;
}

char fileToRam(rmdata *machine, char *file)
{
	unsigned char *ram = machine->ram;
	unsigned char line[256];
	FILE *fp;
	int pos, len, i;
	fp = fopen(file, "rb");
	if(fp == NULL || !allocateRam(machine))
	{	
		fclose(fp);
		return 0;
	}
	
	while(fgets(line, 256, fp) != NULL)
    {
		sscanf(line, "%x=%s", &pos, &line);
		len = strlen(line);
		for(i = pos; i < len / 2 + pos; i++)
		{
			ram[absoluteAdress(machine,i)] = atoh(&line[(i - pos) * 2])<<4 | atoh(&line[(i - pos) * 2 + 1]);
		}
	}
	fclose(fp);
	return 1;
}

char allocateRam(rmdata *machine)
{
	//return 0 if not enough ram
	int i,h;
	int freeBlocks[16],freeBlocksCount=0;
	for (i=0; i<16; i++)
		freeBlocks[i]=-1;

	srand ( (unsigned)time(NULL) );

	for (i=0; i<16; i++)
	{
		for (h=0; h<16; h++)
			if (machine->ram[i*16+h]!=0xFF)
				return 0;
		freeBlocks[freeBlocksCount]=i;
		freeBlocksCount++;
	}
	freeBlocksCount--;
	machine->PTR = freeBlocks[freeBlocksCount];
	
	h=4;
	while (h>0)
	{
		int rndBlock = rand()%(freeBlocksCount);
		if (freeBlocks[rndBlock]!=-1)
		{
			if (h==4)
				machine->IP=rndBlock*16;
			machine->ram[machine->PTR*16+4-h]=rndBlock;
			freeBlocksCount--;
			freeBlocks[rndBlock]=-1;
			h--;
		}
	}
	return 1;
}
int absoluteAdress(rmdata *machine, int virtualAdress)
{
	return 16*(machine->ram[machine->PTR*16+virtualAdress/16])+virtualAdress%16;
}

void updateMachine(rmdata *rmachine, vmdata *vmachine)
{
	int i;
	for (i=0;i<64;i++)
		vmachine->ram[i]=rmachine->ram[absoluteAdress(rmachine,i)];

	vmachine->AX = rmachine->AX;
	vmachine->BX = rmachine->BX;
	//vmachine->IP = absoluteAdress(rmachine,rmachine->IP);
	rmachine->IP = absoluteAdress(rmachine,vmachine->IP); //update real ip
	vmachine->SF = rmachine->SF;
}