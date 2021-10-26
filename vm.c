#include <stdio.h>
#include <stdlib.h>
#include "codegen.h"

int stack[1000] = {0};

int base(int l, int baseloc) // l stand for L in the instruction format
{
  int b1; // find base L levels up
  b1 = baseloc;
  while (l > 0)
  {
    b1 = stack[b1];
    l--;
  }
  return b1;
}

void virtual_machine(instruction *code, int vCheck)
{
	 int SP = 1000;
	int BP = SP - 1;
	int PC = 0;
	int IR = 0;
	int i = 0;
	int RF[8];
	for(i = 0; i < 8; i++)
		RF[i] = 0;
	int halt = 1;
	
	int testCounter = 0;
	int wonkPrint = 0;
  	int stackPrint = 0, stackPrintCounter = 0, stackStepCount = 0, iterateStackPrint;
  	int stackCount[500];
  
  if(vCheck == 1) {
	printf("\n\n");
	printf("Initial Values\t\t\tPC  BP  SP\n");
	printf("Registers: %d %d %d %d %d %d %d %d\t%d  %d %d", RF[0], RF[1], RF[2], RF[3], RF[4], RF[5], RF[6], RF[7], PC, BP, SP);
	printf("\nStack: %d %d %d %d %d %d %d %d %d %d\n\n",
         stack[999], stack[999], stack[998], stack[997], stack[996], stack[995], stack[994], stack[993], stack[992], stack[991]);
  }
  
  	while(halt != 0)
  	{
		//check if current opcode asks to print or recieve an input
		int currentOP = code[PC].opcode;
		if(currentOP == 9)
		{
		if(code[PC].m == 1 || code[PC].m == 2)
			{
			wonkPrint = 1;
		}
		}
		if(wonkPrint == 0 && vCheck == 1)
		{
		// Print r, l and m if the opcode does not print or recieve an input
			switch(currentOP)
			{
				case 1:
					printf("%d %s %d %d %d\t\t\t", PC, "LIT", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 2:
					printf("%d %s %d %d %d\t\t\t", PC, "RTN", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 3:
					printf("%d %s %d %d %d\t\t\t", PC, "LOD", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 4:
					printf("%d %s %d %d %d\t\t\t", PC, "STO", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 5:
					printf("%d %s %d %d %d\t\t\t", PC, "CAL", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 6:
					printf("%d %s %d %d %d\t\t\t", PC, "INC", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 7:
					printf("%d %s %d %d %d\t\t\t", PC, "JMP", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 8:
					printf("%d %s %d %d %d\t\t\t", PC, "JPC", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 9:
					printf("%d %s %d %d %d\t\t\t", PC, "SYS", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 10:
					printf("%d %s %d %d %d\t\t\t", PC, "NEG", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 11:
					printf("%d %s %d %d %d\t\t\t", PC, "ADD", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 12:
					printf("%d %s %d %d %d\t\t\t", PC, "SUB", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 13:
					printf("%d %s %d %d %d\t\t\t", PC, "MUL", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 14:
					printf("%d %s %d %d %d\t\t\t", PC, "DIV", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 15:
					printf("%d %s %d %d %d\t\t\t", PC, "ODD", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 16:
					printf("%d %s %d %d %d\t\t\t", PC, "MOD", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 17:
					printf("%d %s %d %d %d\t\t\t", PC, "EQL", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 18:
					printf("%d %s %d %d %d\t\t\t", PC, "NEQ", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 19:
					printf("%d %s %d %d %d\t\t\t", PC, "LSS", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 20:
					printf("%d %s %d %d %d\t\t\t", PC, "LEQ", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 21:
					printf("%d %s %d %d %d\t\t\t", PC, "GTR", code[PC].r, code[PC].l, code[PC].m);
					break;
				case 22:
					printf("%d %s %d %d %d\t\t\t", PC, "GEQ", code[PC].r, code[PC].l, code[PC].m);
					break;
			}
		}

		// Perform operator on stack
		switch(currentOP)
		{
			case 1:
				// LIT	r, 0, m
				// Loads a constant value (literal) m into Register r
				RF[code[PC].r] = code[PC].m;
				PC++;
				break;

			case 2:
				// RTN 0,0,0
				// offset m from l lexicographical levels up
				SP = BP + 1;
				BP = stack[SP-2];
				PC = stack[SP-3];
				stackPrint--;
				break;

			case 3:
				// LOD	r, l, m
				//  Load value into a selected register from the stack location at
				//      offset m from l lexicographical levels up
				RF[code[PC].r] = stack[base(code[PC].l,BP) - code[PC].m];
				PC++;
				break;

			case 4:
				// STO	r, l, m
				// Store value from a selected register in the stack location at
				//      offset m from l lexicographical levels up
				stack[base(code[PC].l, BP) - code[PC].m] = RF[code[PC].r];
				PC++;
				break;

			case 5:
				// CAL 0,l,m
				// Call procedure at code index m (generates new Activation Record
				//      and PC<--m)
				stack[SP-1] = base(code[PC].l, BP); // Static Link (SL)
				stack[SP-2] = BP;    // Dynamic Link (DL)
				stack[SP-3] = PC + 1;     // Return Address (RA)
				BP = SP - 1;
				PC = code[PC].m;
				break;

			case 6:
				//INC 0,0,m
				// Allocate m memory words (increment SP by m).
				// First three reserved to Static Link (SL), Dynaminc Link (DL)
				//       and Return Address(RA);
				SP = SP - code[PC].m;
				stackCount[stackPrint] = code[PC].m;
				stackPrint++;
				PC++;
				break;

			case 7:
				// JMP 0,0,m
				// Jump to instruction m(PC<--m)
				PC = code[PC].m;
				break;

			case 8:
				// JPC r,0,m
				// Jump to instruction m if r = 0
				if (RF[code[PC].r] == 0)
				{
					PC = code[PC].m;
				}
				else
				{
					PC++;
				}
				break;

			case 9:
				// SYS r,0,1 Write register to screen
				// SYS r,0,2 Read in input from user and store it in register
				// SYS 0,0,3 End of program (set Halt flag to zero)
				if(code[PC].m == 1)
				{
					printf("Register %d: %d\n\n", code[PC].r, RF[code[PC].r]);
				}
				else if(code[PC].m == 2)
				{
					printf("\nPlease enter an integer: ");
					scanf("%d", &RF[code[PC].r]);
				}
				else if(code[PC].m == 3)
				{
					halt = 0;
				}
				PC++;
				break;

			case 10:
				// NEG Change the sign of the given value
				// r, l, m (RF[r] <- RF[r])
				RF[code[PC].r] = -RF[code[PC].r];
				PC++;
				break;

			case 11:
				// ADD 	Add values
				// r, l, m   (RF[r] <- RF[l] + RF[m])
				RF[code[PC].r] = RF[code[PC].l] + RF[code[PC].m];
				PC++;
				break;

			case 12:
				// SUB Subtract values
				// r, l, m   (RF[r] <- RF[l] - RF[m])
				RF[code[PC].r] = RF[code[PC].l] - RF[code[PC].m];
				PC++;
				break;

			case 13:
				// MUL Multiply values
				// r, l, m   (RF[r] <- RF[l] * RF[m])
				RF[code[PC].r] = RF[code[PC].l] * RF[code[PC].m];
				PC++;
				break;

			case 14:
				// DIV 	Divide values
				// r, l, m   (RF[r] <- RF[l] / RF[m])
				RF[code[PC].r] = RF[code[PC].l] / RF[code[PC].m];
				PC++;
				break;

			case 15:
				// ODD Check if the value in register is odd or even
				// r, l, m   (RF[r] <- RF[r] mod 2) or ord(odd(RF[r]))
				RF[code[PC].r] = RF[code[PC].r]%2;
				PC++;
				break;

			case 16:
				// MOD Perform mod operator
				// r, l, m   (RF[r] <- RF[l] mod RF[m])
				RF[code[PC].r] = RF[code[PC].l]%RF[code[PC].m];
				PC++;
				break;

			case 17:
				// EQL Check if values are equal
				// r, l, m   (RF[r] <- RF[l] = = RF[m])
				if(RF[code[PC].l] == RF[code[PC].m])
				{
					// Set as true
					RF[code[PC].r] = 1;
				}
				else
				{
					// Set as false
					RF[code[PC].r] = 0;
				}
				PC++;
				break;
			case 18:
				// NEQ Check if values are not equal
				// r, l, m   (RF[r] <- RF[l] != RF[m])
				if(RF[code[PC].l] != RF[code[PC].m])
				{
					// Set as true
					RF[code[PC].r] = 1;
				}
				else
				{
					// Set as false
					RF[code[PC].r] = 0;
				}
				PC++;
				break;

			case 19:
				// LSS Check to see if value in the lexicographical position is less than
				//      the constant value
				// r, l, m   (RF[r] <- RF[l] < RF[m])
				if(RF[code[PC].l] < RF[code[PC].m])
				{
					// Set as true
					RF[code[PC].r] = 1;
				}
				else
				{
					// Set as false
					RF[code[PC].r] = 0;
				}
				PC++;
				break;

			case 20:
				// LEQ Check to see if value in the lexicographical position is less than
				//      or equal to the constant value
				// r, l, m   (RF[r] <- RF[l] <= RF[m])])
				if(RF[code[PC].l] <= RF[code[PC].m])
				{
					// Set as true
					RF[code[PC].r] = 1;
				}
				else
				{
					// Set as false
					RF[code[PC].r] = 0;
				}
				PC++;
				break;

			case 21:
				// GTR r, l, m  Check to see if value in the lexicographical position is greater than
				//      the constant value
				// r, l, m  (RF[r] <- RF[l] > RF[m])
				if(RF[code[PC].l] > RF[code[PC].m])
				{
					// Set as true
					RF[code[PC].r] = 1;
				}
				else
				{
					// Set as false
					RF[code[PC].r] = 0;
				}
				PC++;

				break;
			case 22:
				// GEQ Check to see if value in the lexicographical position is greater than
				//      or equal to the constant value
				// r, l, m   (RF[r] <- RF[l] >= RF[m])
				if(RF[code[PC].l] >= RF[code[PC].m])
				{
					// Set as true
					RF[code[PC].r] = 1;
				}
				else
				{
					// Set as false
					RF[code[PC].r] = 0;
				}
				PC++;

				break;
		}
		// Print r, l and m if the opcode asks to print or recieve an input
		if(wonkPrint == 1 && vCheck == 1)
		{
			switch(currentOP)
			{
				case 1:
					printf("%d %s %d %d %d\t\t\t", PC-1, "LIT", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 2:
					printf("%d %s %d %d %d\t\t\t", PC-1, "RTN", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 3:
					printf("%d %s %d %d %d\t\t\t", PC-1, "LOD", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 4:
					printf("%d %s %d %d %d\t\t\t", PC-1, "STO", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 5:
					printf("%d %s %d %d %d\t\t\t", PC-1, "CAL", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 6:
					printf("%d %s %d %d %d\t\t\t", PC-1, "INC", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 7:
					printf("%d %s %d %d %d\t\t\t", PC-1, "JMP", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 8:
					printf("%d %s %d %d %d\t\t\t", PC-1, "JPC-1", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 9:
					printf("%d %s %d %d %d\t\t\t", PC-1, "SYS", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 10:
					printf("%d %s %d %d %d\t\t\t", PC-1, "NEG", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 11:
					printf("%d %s %d %d %d\t\t\t", PC-1, "ADD", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 12:
					printf("%d %s %d %d %d\t\t\t", PC-1, "SUB", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 13:
					printf("%d %s %d %d %d\t\t\t", PC-1, "MUL", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 14:
					printf("%d %s %d %d %d\t\t\t", PC-1, "DIV", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 15:
					printf("%d %s %d %d %d\t\t\t", PC-1, "ODD", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 16:
					printf("%d %s %d %d %d\t\t\t", PC-1, "MOD", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 17:
					printf("%d %s %d %d %d\t\t\t", PC-1, "EQL", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 18:
					printf("%d %s %d %d %d\t\t\t", PC-1, "NEQ", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 19:
					printf("%d %s %d %d %d\t\t\t", PC-1, "LSS", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 20:
					printf("%d %s %d %d %d\t\t\t", PC-1, "LEQ", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 21:
					printf("%d %s %d %d %d\t\t\t", PC-1, "GTR", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
				case 22:
					printf("%d %s %d %d %d\t\t\t", PC-1, "GEQ", code[PC-1].r, code[PC-1].l, code[PC-1].m);
					break;
			}
		}
		// Print contents of the register
    if(vCheck == 1) {
		printf("%d %d %d\n", PC, BP, SP);
		printf("Registers: %d %d %d %d %d %d %d %d\n", RF[0], RF[1], RF[2], RF[3], RF[4], RF[5], RF[6], RF[7]);
		printf("Stack: ");
		iterateStackPrint = stackPrint;

		// Print out the stack
		for(i = 999; i >= SP; i--)
		{
		printf("%d ", stack[i]);
		if(iterateStackPrint > 1)
			{
				stackPrintCounter++;
				if(stackPrintCounter == stackCount[stackStepCount])
				{
					printf("| ");
					stackStepCount++;
					stackPrintCounter = 0;
					iterateStackPrint--;
				}
			}
		}

		printf("\n\n");
		stackStepCount = 0, stackPrintCounter = 0, wonkPrint = 0;
  }
	}
	return;
}