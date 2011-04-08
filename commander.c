#include "commander.h"

int executeCode(rmdata *machine, vmdata *vmachine, int offset, int offset2)
{
	unsigned char command = machine->ram[offset];
	switch(command)
	{
		case '+':
			machine->AX = machine->AX + machine->BX;
			return 0;
		case '-':
			machine->AX = machine->AX - machine->BX;
			return 0;
		case 'A':
			machine->AX = machine->AX + machine->ram[absoluteAdress(machine, machine->ram[offset2])];
			return 1;
		case 'a':
			machine->BX = machine->BX + machine->ram[absoluteAdress(machine, machine->ram[offset2])];
			return 1;
		case 'Z':
			machine->AX = machine->AX - machine->ram[absoluteAdress(machine, machine->ram[offset2])];
			return 1;
		case 'z':
			machine->BX = machine->BX - machine->ram[absoluteAdress(machine, machine->ram[offset2])];
			return 1;
		case '=':
			if(machine->AX == machine->BX) machine->SF = 1;
			else machine->SF = 0;
			return 0;
		case 'C':
			if(machine->AX == machine->ram[absoluteAdress(machine, machine->ram[offset2])]) machine->SF = 1;
			else machine->SF = 0;
			return 1;
		case 'c':
			if(machine->BX == machine->ram[absoluteAdress(machine, machine->ram[offset2])]) machine->SF = 1;
			else machine->SF = 0;
			return 1;
		case 'B':
			if(machine->SF == 1)
			{
				vmachine->IP = machine->ram[offset2];
				return 'J';
			}
			else
				return 1;
		case 'b':
			if(machine->SF == 0) 
			{
				vmachine->IP = machine->ram[offset2];
				return 'J';
			}
			else
				return 1;
		case 'J':
			vmachine->IP = machine->ram[offset2];
			return 'J';
		case 'H':
			return 'H';
		case 'L':
			machine->AX = machine->ram[absoluteAdress(machine, machine->ram[offset2])];
			return 1;
		case 'l':
			machine->BX = machine->ram[absoluteAdress(machine, machine->ram[offset2])];
			return 1;
		case 'S':
			machine->ram[absoluteAdress(machine, machine->ram[offset2])] = machine->AX;
			return 1;
		case 's':
			machine->ram[absoluteAdress(machine, machine->ram[offset2])] = machine->BX;
			return 1;
		case 'G':
			return 'G';
		case 'P':
			return 'P';
		default:
			return 255;
	}
}