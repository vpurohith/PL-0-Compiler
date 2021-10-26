#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"
#include "codegen.h"

#define CODE_SIZE 500

int cx = 0;
int tokenCounter = 0;
int tableIndex = 0;
instruction *code;
int savedSymbolIndex = 0;
int savedCondIndex = 0;
int savedJumpIndex = 0;
int iterateIndex = 0;
int procCounterIndex = 500;
char procName[11] = "main";

void emit(int op, int r, int l, int m);
void programGen(symbol *table, lexeme *list);
void blockGen(symbol *table, lexeme *list, int lexLevel, char procedureName[]);
void statementGen(symbol *table, lexeme *list, int lexLevel);
void conditionGen(symbol *table, lexeme *list, int lexLevel);
void expressionGen(symbol *table, lexeme *list, int regToEnd, int lexLevel);
void termGen(symbol *table, lexeme *list, int regToEnd, int lexLevel);
void factorGen(symbol *table, lexeme *list, int regToEnd, int lexLevel);
int searchTable(symbol *table, char* item);


instruction* generate_code(symbol *table, lexeme *list)
{
	code = malloc(500 * sizeof(instruction));

	programGen(table, list);

	return code;
}

void programGen(symbol *table, lexeme *list) {
	int i = 1, procCounter = 0;
	for(int q = 0; q < 500; q++) {
		if(table[q].kind == 3) {
			table[q].val = i;
			i++;
			procCounter++;
			emit(7, 0, 0, 0);
		}
	}
	blockGen(table, list, 0, procName);	

	int procValue = 1, temp;
	
	for(i = 0; i < procCounter; i++) {
		for(int g = 0; g < 500; g++) {
			if(table[g].val == i + 1 && table[g].kind == 3) {
				code[i].m = table[g].addr;
				break;
			}
		}
	} 
	/*
	for(int f = 0; f < 500; f++) {
		if(code[f].opcode == 5) {
			for(int g = 0; g < 500; g++) {
				if(table[g].val == code[f].m && table[g].kind == 3) {
					code[f].m = table[g].addr;
					break;
				}
			}
		}
	} */
	emit(9, 0, 0, 3); //HALT
}

void blockGen(symbol *table, lexeme *list, int lexLevel, char procedureName[]) {
	int numSymbols = 0;
	int numVars = 0;
	if(list[tokenCounter].token == 28) { // const
		do {
			tokenCounter++;
			numSymbols++;
			for(int q = 0; q < 500; q++) {
				if(table[q].kind == 1 && strcmp(list[tokenCounter].lexeme, table[q].name) == 0) {
					table[q].mark == 0;
					break;
				}
			}
			tokenCounter += 3;
		}
		while(list[tokenCounter].token == 17);
		tokenCounter++;
	}
	
	if(list[tokenCounter].token == 29) { // var
		do {
			tokenCounter++;
			numVars++;
			numSymbols++;
			for(int q = 0; q < 500; q++) {
				if(table[q].kind == 2 && strcmp(list[tokenCounter].lexeme, table[q].name) == 0) {
					table[q].mark == 0;
					break;
				}
			}
			tokenCounter++;
		}
		while(list[tokenCounter].token == 17);
		tokenCounter++;
	}

	if(list[tokenCounter].token == 30) { // procedure
		do {
			tokenCounter++;
			strcpy(procName, list[tokenCounter].lexeme);
			numSymbols++;
			for(int q = 0; q < 500; q++) {
				if(table[q].kind == 3 && strcmp(list[tokenCounter].lexeme, table[q].name) == 0) {
					table[q].mark == 0;
					break;
				}
			}
			tokenCounter += 2;
			blockGen(table, list, lexLevel + 1, procName);
			emit(2, 0, 0, 0);
			//tokenCounter++;
		}
		while(list[tokenCounter].token == 30);
		tokenCounter++;
	}

	int p = 0;
	for(p = procCounterIndex; p >= 0; p--) {
		if(table[p].kind == 3) {
			table[p].addr = cx;
			break;
		}
	}
	p--;
	procCounterIndex = p;


	emit(6, 0, 0, 3 + numVars); //INC
	
	statementGen(table, list, lexLevel);

	for(int q = numSymbols; q >= 0; q--) {
		if(table[q].mark == 0) {
			table[q].mark == 1;
			numSymbols--;
		}
		if(numSymbols == 0) break;
	}
}

void statementGen(symbol *table, lexeme *list, int lexLevel) {
	int savedIndex1 = 0;
	int savedIndex2 = 0;
	// token is identifier
	if(list[tokenCounter].token == 2) {
		for(iterateIndex = 500; iterateIndex >= 0; iterateIndex--) {
			if(table[iterateIndex].mark == 0 && table[iterateIndex].kind == 2 && table[iterateIndex].level <= lexLevel)
				break;
		}
		int tableIndex = iterateIndex;
		tokenCounter = tokenCounter + 2;
		expressionGen(table, list, 0, lexLevel);
		emit(4, 0, lexLevel - table[tableIndex].level, table[tableIndex].addr); // STO
	}
	// token is call
	if(list[tokenCounter].token == 27) {
		tokenCounter++;
		for(iterateIndex = 0; iterateIndex < 500; iterateIndex++) {
			if(strcmp(table[iterateIndex].name, list[tokenCounter].lexeme) == 0 && table[iterateIndex].mark == 0 && table[iterateIndex].kind == 3)
				break;
		}
		int tableIndex = iterateIndex;
		emit( 5, 0, lexLevel - table[tableIndex].level, table[tableIndex].addr); // CAL
		tokenCounter++;
	}
	// token is begin
	if(list[tokenCounter].token == 21) {
		tokenCounter++;
		statementGen(table, list, lexLevel);
		while(list[tokenCounter].token == 18) {
			tokenCounter++;
			statementGen(table, list, lexLevel);
		}
		tokenCounter++;
	}
	// token is if
	if(list[tokenCounter].token == 23) {
		tokenCounter++;
		conditionGen(table, list, lexLevel);
		savedIndex1 = cx;
		emit(8, 0, 0, 0); // JPC
		tokenCounter++;
		statementGen(table, list, lexLevel);
		if(list[tokenCounter].token == 33) {
			tokenCounter++;
			savedIndex2 == cx;
			emit(7, 0, 0, 0); // JMP
			code[savedIndex1].m = cx;
			statementGen(table, list, lexLevel);
			code[savedIndex2].m = cx;
		}
		else {
			code[savedIndex1].m = cx;
		}
	}
	// token is while
	if(list[tokenCounter].token == 25) {
		tokenCounter++;
		savedCondIndex = cx;
		conditionGen(table, list, lexLevel);
		tokenCounter++;
		savedJumpIndex = cx;
		emit(8, 0, 0, 0); // JPC
		statementGen(table, list, lexLevel);
		emit(7, 0, 0, savedCondIndex); // JMP
		code[savedJumpIndex].m = cx;
	}
	// token is read
	if(list[tokenCounter].token == 32) {
		tokenCounter++;
		for(iterateIndex = 0; iterateIndex < 500; iterateIndex++) {
			if(table[iterateIndex].mark == 0 && table[iterateIndex].kind == 2 && table[iterateIndex].level <= lexLevel)
				break;
		}
		int tableIndex = iterateIndex;
		tokenCounter++;
		emit(9, 0, 0, 2); // READ
		emit(4, 0, lexLevel - table[tableIndex].level, table[tableIndex].addr); // STO
	}
	// token is write
	if(list[tokenCounter].token == 31) {
		tokenCounter++;
		expressionGen(table, list, 0, lexLevel);
		emit(9, 0, 0, 1); // WRITE
		//statementGen(table, list, lexLevel);
	}
}

void conditionGen(symbol *table, lexeme *list, int lexLevel) {
	if(list[tokenCounter].token == 8) {
		tokenCounter++;
		expressionGen(table, list, 0, lexLevel);
		emit(15, 0, 0, 0); // ODD
	}
	else {
		expressionGen(table, list, 0, lexLevel);
		if(list[tokenCounter].token == 9) {
			tokenCounter++;
			expressionGen(table, list, 1, lexLevel);
			emit(17, 0, 0, 1); // EQL
		}
		if(list[tokenCounter].token == 10) {
			tokenCounter++;
			expressionGen(table, list, 1, lexLevel);
			emit(18, 0, 0, 1); // NEQ
		}
		if(list[tokenCounter].token == 11) {
			tokenCounter++;
			expressionGen(table, list, 1, lexLevel);
			emit(19, 0, 0, 1); // LSS
		}
		if(list[tokenCounter].token == 12) {
			tokenCounter++;
			expressionGen(table, list, 1, lexLevel);
			emit(20, 0, 0, 1); // LEQ
		}
		if(list[tokenCounter].token == 13) {
			tokenCounter++;
			expressionGen(table, list, 1, lexLevel);
			emit(21, 0, 0, 1); // GTR
		}
		if(list[tokenCounter].token == 14) {
			tokenCounter++;
			expressionGen(table, list, 1, lexLevel);
			emit(22, 0, 0, 1); // GEQ
		}
	}
}

void expressionGen(symbol *table, lexeme *list, int regToEnd, int lexLevel) {
	if(list[tokenCounter].token == 4) {
		tokenCounter++;
	}
	if(list[tokenCounter].token == 5) {
		tokenCounter++;
		termGen(table, list, regToEnd, lexLevel);
		emit(10, regToEnd, 0, 0); // NEG
		while(list[tokenCounter].token == 4 || list[tokenCounter].token == 5) {
			if(list[tokenCounter].token == 4) {
				tokenCounter++;
				termGen(table, list, regToEnd + 1, lexLevel);
				emit(11, regToEnd, regToEnd, regToEnd + 1); // ADD
			}
			if(list[tokenCounter].token == 5) {
				tokenCounter++;
				termGen(table, list, regToEnd + 1, lexLevel);
				emit(12, regToEnd, regToEnd, regToEnd + 1); // SUB
			}
		}
		return;
	}
	termGen(table, list, regToEnd, lexLevel);
	while(list[tokenCounter].token == 4 || list[tokenCounter].token == 5) {
		if(list[tokenCounter].token == 4) {
			tokenCounter++;
			termGen(table, list, regToEnd + 1, lexLevel);
			emit(11, regToEnd, regToEnd, regToEnd + 1); // ADD
		}
		if(list[tokenCounter].token == 5) {
			tokenCounter++;
			termGen(table, list, regToEnd + 1, lexLevel);
			emit(12, regToEnd, regToEnd, regToEnd + 1); // SUB
		}
	}
}

void termGen(symbol *table, lexeme *list, int regToEnd, int lexLevel) {
	factorGen(table, list, regToEnd, lexLevel);
	while(list[tokenCounter].token == 6 || list[tokenCounter].token == 7) {
		if(list[tokenCounter].token == 6) {
			tokenCounter++;
			factorGen(table, list, regToEnd + 1, lexLevel);
			emit(13, regToEnd, regToEnd, regToEnd + 1); // MUL
		}
		if(list[tokenCounter].token == 7) {
			tokenCounter++;
			factorGen(table, list, regToEnd + 1, lexLevel);
			emit(14, regToEnd, regToEnd, regToEnd + 1); // DIV
		}
	}
}

void factorGen(symbol *table, lexeme *list, int regToEnd, int lexLevel) {
	if(list[tokenCounter].token == 2) {
		for(iterateIndex = 0; iterateIndex < 500; iterateIndex++) {
			if(table[iterateIndex].mark == 0 && table[iterateIndex].kind < 3 && table[iterateIndex].level <= lexLevel)
				break;
		}
		int tableIndex = iterateIndex;
		if(table[tableIndex].kind == 1) {
			emit(1, regToEnd, 0, table[tableIndex].val); // LIT
		}
		if(table[tableIndex].kind == 2) {
			emit(3, regToEnd, lexLevel - table[tableIndex].level, table[tableIndex].addr); // LOD
		}
		tokenCounter++;
	}
	else if(list[tokenCounter].token == 3) {
		emit(1, regToEnd, 0, list[tokenCounter].number);
		tokenCounter++;
	}
	else {
		tokenCounter++;
		expressionGen(table, list, regToEnd, lexLevel);
		tokenCounter++;

	}
}



int searchTable(symbol *table, char* item) {
	int i = 0;
	for(i = 0; i < 500; i++) {
		if(strcmp(table[i].name, item) == 0)
			break;
	}
	if(i = 500) return -1;
	else return i;
}


void emit(int op, int r, int l, int m) {
	if(cx > CODE_SIZE) {
		printf("\nError: too much code");
		exit(0);
	}
	else {
		code[cx].opcode = op;
		code[cx].r = r;
		code[cx].l = l;
		code[cx].m = m;
		cx++;
	}
}

