#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"
#include "lex.h"

void program(lexeme *list, symbol *table);	
void block(lexeme *list, symbol *table, int lexLevel);
int constantDeclare(lexeme *list, symbol *table, int lexLevel);
int varDeclare(lexeme *list, symbol *table, int lexLevel);
int procDeclare(lexeme *list, symbol *table, int lexLevel);
void statement(lexeme *list, symbol *table, int lexLevel);
void condition(lexeme *list, symbol *table, int lexLevel);
void expression(lexeme *list, symbol *table, int lexLevel);
void term(lexeme *list, symbol *table, int lexLevel);
void factor(lexeme *list, symbol *table, int lexLevel);

int listCounter = 0;
int tableCounter = 0;
int m = 0;

symbol* parse(lexeme *list)
{
	symbol *table = malloc(500 * sizeof(symbol));

	program(list, table);

	return table;
}

void program(lexeme *list, symbol *table) {
	table[tableCounter].kind = 3;
			strcpy(table[tableCounter].name, "main");
			table[tableCounter].val = 0;
			table[tableCounter].level = 0;
			table[tableCounter].addr = 0;
			table[tableCounter].mark = 0;
			tableCounter++;
	block(list, table, 0);
	if(strcmp(list[listCounter].lexeme, ".") != 0) {
		printf("\nError: program does not end in period\n\n");
		exit(0);
	}
}

void block(lexeme *list, symbol *table, int lexLevel) {
	int numSymbols = 0;
	numSymbols += constantDeclare(list, table, lexLevel);
	numSymbols += varDeclare(list, table, lexLevel);
	numSymbols += procDeclare(list, table, lexLevel);
	statement(list, table, lexLevel);
	for(int q = tableCounter; q >= 0; q--) {
		if(table[q].mark == 0) {
			table[q].mark == 1;
			numSymbols--;
		}
		if(numSymbols == 0) break;
	}
}

int constantDeclare(lexeme *list, symbol *table, int lexLevel) {
	int numConst = 0;
	if(list[listCounter].token == 28) {
		do {
			listCounter++;
			if(list[listCounter].token != 2) {
					printf("\nError: constant is not variable\n\n");
					exit(0);
			}
      char nameCache[12];
			strcpy(nameCache, list[listCounter].lexeme);
      for(m = 0; m < tableCounter; m++) {
			  if(strcmp(table[m].name, list[listCounter].lexeme) == 0)
				  break;
		  }
			if(m < tableCounter && table[m].mark == 0 && table[m].level == lexLevel) {
				printf("\nError: variable already in table at same lex level\n\n");
				exit(0);
			}
      m = 0;
			listCounter++;
			if(list[listCounter].token != 9) {
				printf("\nError: invalid syntax when making a constant\n\n");
				exit(0);
			}
			listCounter++;
			if(list[listCounter].token != 3) {
				printf("\nError: nonnumber is assigned to constant\n\n");
				exit(0);
			}
			table[tableCounter].kind = 1;
			strcpy(table[tableCounter].name, nameCache);
			table[tableCounter].val = list[listCounter].number;
			table[tableCounter].level = lexLevel;
			table[tableCounter].addr = 0;
			table[tableCounter].mark = 0;
			tableCounter++;
			listCounter++;
			numConst++;
		}
		while(list[listCounter].token == 17);
		if(list[listCounter].token != 18) {
			printf("\nError: line does not end in semicolon 1\n\n");
			exit(0);
		}
		listCounter++;
	}
	return numConst;
}

int varDeclare(lexeme *list, symbol *table, int lexLevel) {
	int numVars = 0;
	if(list[listCounter].token == 29) {
		int numVars = 0;
		do {
			numVars++;
			listCounter++;
			if(list[listCounter].token != 2) {
					printf("\nError: variable is incorrectly assigned\n\n");
          			exit(0);
     		}
			char nameCache[12];
			strcpy(nameCache, list[listCounter].lexeme);
      		for(m = 0; m < tableCounter; m++) {
			  if(strcmp(table[m].name, list[listCounter].lexeme) == 0)
				  break;
		  }
		if(m < tableCounter && table[m].mark == 0 && table[m].level == lexLevel) {
			printf("\nError: variable already in table at same lex level\n\n");
        	exit(0);
      	}
      		m = 0;
			table[tableCounter].kind = 2;		
			strcpy(table[tableCounter].name, nameCache);
			table[tableCounter].val = 0;
			table[tableCounter].level = lexLevel;
			table[tableCounter].addr = numVars + 2;
			table[tableCounter].mark = 0;
			tableCounter++;
			listCounter++;
		}
		while(list[listCounter].token == 17);
		if(list[listCounter].token != 18) {
			printf("\nError: line does not end in semicolon 2\n\n");
      exit(0);
     }	
		listCounter++;
	}
	 return numVars;
}

int procDeclare(lexeme *list, symbol *table, int lexLevel) {
	int numProcedures = 0;
	if(list[listCounter].token == 30) {
		do {
			numProcedures++;
			listCounter++;
			if(list[listCounter].token != 2) {
				printf("\nError: procedure is incorrectly assigned\n\n");
          		exit(0);
     		}
			char nameCache[12];	
			strcpy(nameCache, list[listCounter].lexeme);
     		for(m = 0; m < tableCounter; m++) {
			  if(strcmp(table[m].name, list[listCounter].lexeme) == 0)
				  break;
			}
			if(m < tableCounter && table[m].mark == 0 && table[m].level == lexLevel) {
				printf("\nError: procedure already in table\n\n");
       			exit(0);
      		}
			m = 0;
			table[tableCounter].kind = 3;		
			strcpy(table[tableCounter].name, nameCache);
			table[tableCounter].val = 0;
			table[tableCounter].level = lexLevel;
			table[tableCounter].addr = 0;
			table[tableCounter].mark = 0;
			tableCounter++;
			listCounter++;
			if(list[listCounter].token != 18) {
				printf("\nError: line does not end in semicolon 3\n\n");
      			exit(0);
     		}
			listCounter++;
			block(list, table, lexLevel + 1);
			if(list[listCounter].token != 18 && list[listCounter].token != 19) {
				printf("\nError: line does not end in semicolon 4 %d %d\n\n", listCounter, list[listCounter].token);
      			exit(0);
     		}
			listCounter++;
		}
		while(list[listCounter].token == 30);
	}
	return numProcedures;
}

void statement(lexeme *list, symbol *table, int lexLevel) { 
	//token is identifier
	if(list[listCounter].token == 2) {
		char nameCache[12];
		strcpy(nameCache, list[listCounter].lexeme);
		int foundSwitch = 0;
		for (int z = tableCounter; z >= 0; z--) {
			if(table[z].kind == 2 && table[z].mark == 0 && strcmp(table[z].name, list[listCounter].lexeme) == 0) {
				foundSwitch = 1;
				break;
			}
		}
		if (foundSwitch == 0) {
			printf("\nError: variable not in table\n\n");
			exit(0);
		}
		listCounter++;
		if(list[listCounter].token != 20) {
			printf("\nError: statement does not have equals sign\n\n");
      		exit(0);
    	}
		listCounter++;
		expression(list, table, lexLevel);
		return;
	}
	//token is call
	if(list[listCounter].token == 27) {
		listCounter++;
		char nameCache[12];
		strcpy(nameCache, list[listCounter].lexeme);
		int foundSwitch = 0;
		for (int z = tableCounter; z >= 0; z--) {
			if(table[z].kind == 3 && table[z].mark == 0 && strcmp(table[z].name, list[listCounter].lexeme) == 0) {
				foundSwitch = 1;
				break;
			}
		}
		if (foundSwitch == 0) {
			printf("\nError: variable not in table\n\n");
			exit(0);
		}
		listCounter++;
		return;
	}
	//token is "begin"
	if(list[listCounter].token == 21) {
		listCounter++;
		statement(list, table, lexLevel);
		while(list[listCounter].token == 18) {
			listCounter++;
			statement(list, table, lexLevel);
		}
		if(list[listCounter].token != 22) {
			printf("\nError: begin in program does not have end\n\n");
      		exit(0);
    	}
		listCounter++;
		return;
	}
	//token is "if"
	if(list[listCounter].token == 23) {
		listCounter++;
		condition(list, table, lexLevel);
		if(list[listCounter].token != 24) { 
			printf("\nError: if statement does not have then\n\n");
       		exit(0);
    	}
		listCounter++;
		statement(list, table, lexLevel);
		if(list[listCounter].token == 33) {
			listCounter++;
			statement(list, table, lexLevel);
		}
		return;
	}
	//token is "while"
	if(list[listCounter].token == 25) {
		listCounter++;
		condition(list, table, lexLevel);
		if(list[listCounter].token != 26) {
			printf("\nError: while statement does not have do\n\n");
      		exit(0);
    	}
		listCounter++;
		statement(list, table, lexLevel);
		return;
	}
	//token is "read"
	if(list[listCounter].token == 32) {
		listCounter++;
		char nameCache[12];
		strcpy(nameCache, list[listCounter].lexeme);
		int foundSwitch = 0;
		for (int z = tableCounter; z >= 0; z--) {
			if(table[z].kind == 2 && table[z].mark == 0 && strcmp(table[z].name, list[listCounter].lexeme) == 0) {
				foundSwitch = 1;
				break;
			}
		}
		if (foundSwitch == 0) {
			printf("\nError: variable not in table\n\n");
			exit(0);
		}
		listCounter++;
		return;
	}
	//token is "write"
	if(list[listCounter].token == 31) {
		listCounter++;
		expression(list, table, lexLevel);
		return;
	}
	return;
}

void condition(lexeme *list, symbol *table, int lexLevel) {
	if(list[listCounter].token == 8) {
		listCounter++;
		expression(list, table, lexLevel);
	}
	else {
		expression(list, table, lexLevel);
		if(list[listCounter].token < 9 || list[listCounter].token > 14) {
			printf("\nError: improper conditional statement \n\n");
      		exit(0);
    	}
		listCounter++;
		expression(list, table, lexLevel);
	}
}

void expression(lexeme *list, symbol *table, int lexLevel) {
	if(list[listCounter].token == 4 || list[listCounter].token == 5)
		listCounter++;
	term(list, table, lexLevel);
	while(list[listCounter].token == 4 || list[listCounter].token == 5) {
		listCounter++;
		term(list, table, lexLevel);
	}
}

void term(lexeme *list, symbol *table, int lexLevel) {
	factor(list, table, lexLevel);
	while(list[listCounter].token == 6 || list[listCounter].token == 7) {
		listCounter++;
		factor(list, table, lexLevel);
	}
}

void factor(lexeme *list, symbol *table, int lexLevel) {
	if(list[listCounter].token == 2) {
		char nameCache[12];
		strcpy(nameCache, list[listCounter].lexeme);
		int foundSwitch = 0;
		for (int z = tableCounter; z >= 0; z--) {
			if((table[z].kind == 2 || table[z].kind == 1) && table[z].mark == 0 && strcmp(table[z].name, list[listCounter].lexeme) == 0) {
				foundSwitch = 1;
				break;
			}
		}
		if (foundSwitch == 0) {
			printf("\nError: variable not in table \n\n");
			exit(0);
		}
		listCounter++;
	}
	else if(list[listCounter].token == 3)
		listCounter++;
	else if(list[listCounter].token == 15) {
		listCounter++;
		expression(list, table, lexLevel);
		if(list[listCounter].token != 16) {
			printf("\nError: parenthesis is not closed\n\n");
     		exit(0);
    	}
		listCounter++;
	}
	else {
		printf("\nError: incorrect syntax\n\n");
   		exit(0);
    }
}
