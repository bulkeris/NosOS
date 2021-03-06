#include "io.h"

unsigned char getProgramInput()
{
	HANDLE keyboard = GetStdHandle(STD_INPUT_HANDLE);

	ReadConsoleInput(keyboard, input, 1, &w);
	if(input[0].EventType == KEY_EVENT && input[0].Event.KeyEvent.bKeyDown)
	{
		return input[0].Event.KeyEvent.uChar.AsciiChar;
	}
	return 0;
}

void gotoxy(HANDLE handle, int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(handle, coord);
}

void printAllColors()
{
	int i;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for(i = 0; i < 255; i++)
	{
		SetConsoleTextAttribute(hConsole, i);
		printf("#define COLOR %d\n", i);
	}
}

int progSpace(rmdata *realmachine, int i)
{
	int ii;
	for(ii = 0; ii < 16; ii++)
	{
		if((realmachine->ram[realmachine->PTR * 0x10 + ii] * 0x10 <= i) && (realmachine->ram[realmachine->PTR * 0x10 + ii] * 0x10 + 16 > i)  && realmachine->ram[realmachine->PTR * 0x10 + ii] != 0xFF)
		{
			return 1;
		}
	}
	return 0;
}

void draw(rmdata *realmachine, vmdata *virtualmachine)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int i, ii;

	gotoxy(hConsole, 0, 0);
	SetConsoleTextAttribute(hConsole, TITLE);
	printf("Real machine RAM:                                                     Reg:   ");
	for(i = 0; i < 256; i++)
	{
		if(i % 0x10 == 0)
		{
			SetConsoleTextAttribute(hConsole, OFFSET);
			printf("\n%03X: ", i);
		}
		if((realmachine->PTR * 0x10 <= i) && ((realmachine->PTR * 0x10 + 0x10) > i) && realmachine->ram[realmachine->PTR * 0x10] != 0xFF)
			SetConsoleTextAttribute(hConsole, PTRS);
		else if(progSpace(realmachine, i))
			SetConsoleTextAttribute(hConsole, PROG);
		else if(realmachine->ram[i] == 0xFF)
			SetConsoleTextAttribute(hConsole, HEXFF);
		else
			SetConsoleTextAttribute(hConsole, HEX);
		printf("%02X ", realmachine->ram[i]);
		if((i - 0xF) % 0x10 == 0)
		{
			SetConsoleTextAttribute(hConsole, ASCII);
			for(ii = 0; ii < 16; ii++)
			{
				if(realmachine->ram[i - 0xF + ii] >= 0x20 && realmachine->ram[i - 0xF + ii] <= 0x7E)
				{
					printf("%c", realmachine->ram[i - 0xF + ii]);
				}
				else
				{
					printf(".");
				}
			}
			SetConsoleTextAttribute(hConsole, REG);
			if(i - 0xF == 0)
			{
				printf(" AX:  %02X", realmachine->AX);
			}
			else if(i - 0xF == 0x10)
			{
				printf(" BX:  %02X", realmachine->BX);
			}
			else if(i - 0xF == 0x20)
			{
				printf(" IP:  %02X", realmachine->IP);
			}
			else if(i - 0xF == 0x30)
			{
				printf(" SF:  %02X", realmachine->SF);
			}
			else if(i - 0xF == 0x40)
			{
				printf(" PTR: %02X", realmachine->PTR);
			}
			else if(i - 0xF == 0x50)
			{
				printf(" PI:  %02X", realmachine->PI);
			}
			else if(i - 0xF == 0x60)
			{
				printf(" SI:  %02X", realmachine->SI);
			}
			else if(i - 0xF == 0x70)
			{
				printf(" TI:  %02X", realmachine->TI);
			}
			else if(i - 0xF == 0x80)
			{
				printf(" MODE:%02X", realmachine->MODE);
			}
			else if(i - 0xF == 0x90)
			{
				printf(" CN1: %02X", realmachine->CN1);
			}
			else if(i - 0xF == 0xA0)
			{
				printf(" CN2: %02X", realmachine->CN2);
			}
			else if(i - 0xF == 0xB0)
			{
				printf(" CN3: %02X", realmachine->CN3);
			}
			else if(i - 0xF == 0xC0)
			{
				printf(" IOI: %02X", realmachine->IOI);
			}
		}
	}
	SetConsoleTextAttribute(hConsole, TITLE);
	printf("\nVirtual machine RAM:                                                  Reg:   ");



	//FAKE CAR RAM??? NEED TO EDIT
	for(i = 0; i < 64; i++)
	{
		if(i % 0x10 == 0)
		{
			SetConsoleTextAttribute(hConsole, OFFSET);
			printf("\n%03X: ", i);
			if(realmachine->ram[realmachine->PTR * 0x10] != 0xFF)
				SetConsoleTextAttribute(hConsole, PROG);
			else
				SetConsoleTextAttribute(hConsole, HEXFF);
		}
		printf("%02X ", virtualmachine->ram[i]);
		if((i - 0xF) % 0x10 == 0)
		{
			SetConsoleTextAttribute(hConsole, ASCII);
			for(ii = 0; ii < 16; ii++)
			{
				if(virtualmachine->ram[i - 0xF + ii] >= 0x20 && virtualmachine->ram[i - 0xF + ii] <= 0x7E)
				{
					printf("%c", virtualmachine->ram[i - 0xF + ii]);
				}
				else
				{
					printf(".");
				}
			}
			SetConsoleTextAttribute(hConsole, REG);
			if(i - 0xF == 0)
			{
				printf(" AX:  %02X", virtualmachine->AX);
			}
			else if(i - 0xF == 0x10)
			{
				printf(" BX:  %02X", virtualmachine->BX);
			}
			else if(i - 0xF == 0x20)
			{
				printf(" IP:  %02X", virtualmachine->IP);
			}
			else if(i - 0xF == 0x30)
			{
				printf(" SF:  %02X", virtualmachine->SF);
			}
		}
	}


	SetConsoleTextAttribute(hConsole, TITLE);
	if(mode == 1) printf("\nConsole:                                                                     \n");
	else printf("\nProgram:                                                                   \n");
	SetConsoleTextAttribute(hConsole, TYPE);

	if(strlen(text) == 0)
	{
		printf("                                                                               \n");
		gotoxy(hConsole, 0, 23);
	}

	printf("%s", text);
}