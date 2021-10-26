// Vishal Purohitham
// Assignment: Homework 4 (Parser/Code Generator)
// COP 3402 System Software
// Instructor: Montagne
// Due Date: 12/7/20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"
#include "codegen.h"
#include "vm.h"

int main(int argc, char **argv)
{
	if (argc < 2)
		printf("error: please include the file name");
	
	FILE *ifp = fopen(argv[1], "r");
	
	char *inputfile = malloc(500 * sizeof(char));
	char c = fgetc(ifp);
	int i = 0, j = 0, k = 0, codeCount = 0, lineCounter = 0;
	while (1)
	{
		inputfile[i++] = c;
		c = fgetc(ifp);
		if (c == EOF)
			break;
	}
	inputfile[i] = '\0';
 
  int dirCheckL = 0, dirCheckA = 0, dirCheckV = 0;
	
  for(k = 2; k < argc; k++) {
    if(strcmp(argv[k], "-l") == 0) dirCheckL = 1;
    if(strcmp(argv[k], "-a") == 0) dirCheckA = 1;
    if(strcmp(argv[k], "-v") == 0) dirCheckV = 1;
  } 
 
	printf("\nInput File: \n");
	printf("%s\n\n", inputfile);
	
	lexeme *list = lex_analyze(inputfile, i, dirCheckL);

	symbol *table = parse(list);
 
    printf("\n\nNo errors, program is syntactically correct.\n");

	instruction *code = generate_code(table, list);
 
  while(code[codeCount].opcode != 0) {
    codeCount++;
  } 
 
  if(dirCheckA == 1) {
	  printf("\n\n");
    printf("Line\tOP\tR\tL\tM\n");
	  for(int x = 0; x < codeCount; x++) {
      printf("%d\t", x);
        switch(code[x].opcode)
    {
      case 1:
          printf("%s\t", "LIT");
          break;
      case 2:
          printf("%s\t", "RTN");
          break;
      case 3:
          printf("%s\t", "LOD");
          break;
      case 4:
          printf("%s\t", "STO");
          break;
      case 5:
          printf("%s\t", "CAL");
          break;
      case 6:
          printf("%s\t", "INC");
          break;
      case 7:
          printf("%s\t", "JMP");
          break;
      case 8:
          printf("%s\t", "JPC");
          break;
      case 9:
          printf("%s\t", "SYS");
          break;
      case 10:
          printf("%s\t", "NEG");
          break;
      case 11:
          printf("%s\t", "ADD");
          break;
      case 12:
          printf("%s\t", "SUB");
          break;
      case 13:
          printf("%s\t", "MUL");
          break;
      case 14:
          printf("%s\t", "DIV");
          break;
      case 15:
          printf("%s\t", "ODD");
          break;
      case 16:
          printf("%s\t", "MOD");
          break;
      case 17:
          printf("%s\t", "EQL");
          break;
      case 18:
          printf("%s\t", "NEQ");
          break;
      case 19:
          printf("%s\t", "LSS");
          break;
      case 20:
          printf("%s\t", "LEQ");
          break;
      case 21:
          printf("%s\t", "GTR");
          break;
      case 22:
          printf("%s\t", "GEQ");
          break;
    }
    printf("%d\t", code[x].r);
    printf("%d\t", code[x].l);
    printf("%d\n", code[x].m);
	  }
	  printf("\n\n");
  }
	
	virtual_machine(code, dirCheckV); 
	
	return 0;
}