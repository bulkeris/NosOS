#include "main.h"

void callUpdateAndDraw(rmdata *realmachine, vmdata *virtualmachine)
{
	if(realmachine->ram[realmachine->PTR * 0x10] != 0xFF)
		updateMachine(realmachine, virtualmachine);
	draw(realmachine, virtualmachine);
}

int main()
{
	int mark, o;
	unsigned char c;
	rmdata *realmachine;
	vmdata *virtualmachine;
	realmachine = malloc(sizeof(rmdata));
	virtualmachine = malloc(sizeof(vmdata));

	SetConsoleTitle(LTITLE);
	strcpy(text, "");
	mode = 1;

	clearRReg(realmachine, 0);
	clearVReg(virtualmachine, 0);
	clearRRam(realmachine, 0xFF);
	clearVRam(virtualmachine, 0xFF);

	callUpdateAndDraw(realmachine, virtualmachine);
	while(1)
	{
		if(mode == 0)
		{
			sprintf(text, "%sExec command: %X\n", text, realmachine->ram[absoluteAdress(realmachine,virtualmachine->IP)]);
			o = executeCode(realmachine, virtualmachine, absoluteAdress(realmachine,virtualmachine->IP), absoluteAdress(realmachine,virtualmachine->IP + 1));
			if(o == 1)
			{
				virtualmachine->IP++;
			}
			else if(o == 'G')
			{
				realmachine->TI = 1;
				callUpdateAndDraw(realmachine, virtualmachine);
				mark = realmachine->ram[absoluteAdress(realmachine,virtualmachine->IP + 1)];
				c = 0;
				while(c != 0x0D)
				{
					if(c)
					{
						realmachine->ram[absoluteAdress(realmachine, mark)] = c;
						sprintf(text, "%s%c", text, c);
						mark++;
					}
					c = getProgramInput();
					callUpdateAndDraw(realmachine, virtualmachine);
				}
				realmachine->ram[absoluteAdress(realmachine, mark)] = 0;
				strcat(text, "\n");
				virtualmachine->IP++;
				realmachine->TI = 0;
			}
			else if(o == 'P')
			{
				virtualmachine->IP++;
				mark = realmachine->ram[absoluteAdress(realmachine,virtualmachine->IP)];
				while((c = realmachine->ram[absoluteAdress(realmachine, mark)]) != 0x00)
				{
					sprintf(text, "%s%c", text, c);
					mark++;
				}
				strcat(text, "\n");
			}
			else if(o == 'H')
			{
				mode = 2;
				strcat(text, "Program ended successfully.\n");
			}
			else if(o == 255)
			{
				mode = 2;
				realmachine->PI = 1;
				strcat(text, "Error, program ended unsuccessfully!\n");
			}
			if(o != 'J')
			{
				virtualmachine->IP++;
			}
			if(realmachine->MODE)
			{
				strcat(text, "Press ENTER key...");
				callUpdateAndDraw(realmachine, virtualmachine);
				while(getchar() != '\n');
				strcat(text, "\n");
			}
			else
			{
				callUpdateAndDraw(realmachine, virtualmachine);
			}
		}
		else if(mode == 1)
		{
			c = getProgramInput();
			if(c && c != '`' && c != 0x0D)
			{
				sprintf(text, "%s%c", text, c);
			}
			if(c == 0x0D)
			{
				if(strcmp(text, "quit") == 0 || strcmp(text, "exit") == 0 || strcmp(text, "e") == 0 || strcmp(text, "q") == 0)
				{
					break;
				}
				else if(strcmp(text, "start") == 0 || strcmp(text, "s") == 0)
				{
					mode = 0;
				}
				else if(strcmp(text, "step") == 0)
				{
					if(realmachine->MODE)
					{
						realmachine->MODE = 0;
					}
					else
					{
						realmachine->MODE = 1;
					}
				}
				else if(strcmp(text, "reset") == 0 || strcmp(text, "r") == 0)
				{
					clearRReg(realmachine, 0);
					clearVReg(virtualmachine, 0);
					clearRRam(realmachine, 0xFF);
					clearVRam(virtualmachine, 0xFF);
				}
				if(strcmp(text, "cls") == 0)
				{
					system("cls");
				}
				else if(strncmp(text, "loadfile", 8) == 0)
				{
					fileToRam(realmachine, text + 9);
				}
				else if(strcmp(text, "load") == 0 || strcmp(text, "l") == 0)
				{
					fileToRam(realmachine, HDDFILE);
				}
				strcpy(text, "");
			}
			callUpdateAndDraw(realmachine, virtualmachine);
		}
		else if(mode == 2)
		{
			//strcat(text, "Press ENTER key...");
			//callUpdateAndDraw(realmachine, virtualmachine);
			//while(getchar() != '\n');
			system("pause");
			strcpy(text, "");
			mode = 1;
			system("cls");
			callUpdateAndDraw(realmachine, virtualmachine);
		}
	}

	return 0;
}