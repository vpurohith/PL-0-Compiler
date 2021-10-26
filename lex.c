#include "lex.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int identifierFinder(char* lexeme);
int stringToInt(char* convertString);

lexeme* lex_analyze(char *inputfile, int size, int lCheck)
{
	lexeme *list = malloc(500 * sizeof(lexeme));
	int cacheCounter = 0, lexCounter = 0, checkCounter = 0;
    int a = 0, i = 0, j = 0;
    char reservedWords[14][10] = {"call", "else", "procedure", "const", "begin", "end", "if", "then", "var", "while", "do", "read", "write", "odd"}; //procedure, call, else      const var

	for(a = 0; a < size; a++) {
        char cacheArray[12];
        while(isspace(inputfile[a]) || iscntrl(inputfile[a])) {
            a++;
        }
        // determines what to do if the character is a letter
        if(isalpha(inputfile[a])) {  
            cacheArray[cacheCounter] = inputfile[a];
            a++;
            cacheCounter++;
            while(isalpha(inputfile[a]) || isdigit(inputfile[a])) {
                cacheArray[cacheCounter] = inputfile[a];
                a++;
                cacheCounter++;
                // if the string is too long then show an identifier too long error
                if(cacheCounter == 12) {
                    printf("\nError: Identifier is too long");
                    exit(0);
                }
            }
            // if its not too long, check if its a reserved word or identifier
            if(cacheCounter < 12) {
                for(i = 0; i < 14; i++) {
                    if(strcmp(cacheArray, reservedWords[i]) == 0) break;
                }
                if(i < 14) {
                    strcpy(list[lexCounter].lexeme, reservedWords[i]);
                    list[lexCounter].token = identifierFinder(cacheArray);
                }
                else {
                    strcpy(list[lexCounter].lexeme, cacheArray);
                    list[lexCounter].token = 2;
                }
            }

        memset(cacheArray, 0, 12);
        lexCounter++;
        checkCounter++;
        }
        // determines what to do if the character is a number
        else if(isdigit(inputfile[a])) {
            cacheArray[cacheCounter] = inputfile[a];
            a++;
            cacheCounter++;
            while(isdigit(inputfile[a])) {
                cacheArray[cacheCounter] = inputfile[a];
                a++;
                cacheCounter++;
                if(cacheCounter == 6) {
                    printf("\nError: Number is too long");
                    exit(0);
                }
            }
            if(isalpha(inputfile[a]) && cacheCounter < 6) {
                printf("\nError: Invalid identifier");
                exit(0);
            }
            else if(cacheCounter < 6) {
                list[lexCounter].number = stringToInt(cacheArray);
                list[lexCounter].token = 3;
            }
        memset(cacheArray, 0, 12);
        lexCounter++;
        checkCounter++;
        }
        // if the character is +
        if(inputfile[a] == '+') {
            strcpy(list[lexCounter].lexeme, "+");
            list[lexCounter].token = 4;
            lexCounter++;
            checkCounter++;
        }
        // if the character is -
        if(inputfile[a] == '-') {
            strcpy(list[lexCounter].lexeme, "-");
            list[lexCounter].token = 5;
            lexCounter++;
            checkCounter++;
        }
        // if the character is *
        if(inputfile[a] == '*') {
            strcpy(list[lexCounter].lexeme, "*");
            list[lexCounter].token = 6;
            lexCounter++;
            checkCounter++;
        }
        // if the character is =
        if(inputfile[a] == '=') {
            strcpy(list[lexCounter].lexeme, "=");
            list[lexCounter].token = 9;
            lexCounter++;
            checkCounter++;
        }
        // if the character is (
        if(inputfile[a] == '(') {
            strcpy(list[lexCounter].lexeme, "(");
            list[lexCounter].token = 15;
            lexCounter++;
            checkCounter++;
        }
        // if the character is )
        if(inputfile[a] == ')') {
            strcpy(list[lexCounter].lexeme, ")");
            list[lexCounter].token = 16;
            lexCounter++;
            checkCounter++;
        }
        // if the character is ,
        if(inputfile[a] == ',') {
            strcpy(list[lexCounter].lexeme, ",");
            list[lexCounter].token = 17;
            lexCounter++;
            checkCounter++;
        }
        // if the character is ;
        if(inputfile[a] == ';') {
            strcpy(list[lexCounter].lexeme, ";");
            list[lexCounter].token = 18;
            lexCounter++;
            checkCounter++;
        }
        // if the character is .
        if(inputfile[a] == '.') {
            strcpy(list[lexCounter].lexeme, ".");
            list[lexCounter].token = 19;
            lexCounter++;
            checkCounter++;
        }
        // if the character is <
        if(inputfile[a] == '<') {
            a++;
            if(inputfile[a] == '>') {
                strcpy(list[lexCounter].lexeme, "<>");
                list[lexCounter].token = 10;
                lexCounter++;
                checkCounter++;
            }
            else if(inputfile[a] == '=') {
                strcpy(list[lexCounter].lexeme, "<=");
                list[lexCounter].token = 12;
                lexCounter++;
                checkCounter++;
            }
            else {
                strcpy(list[lexCounter].lexeme, "<");
                list[lexCounter].token = 11;
                lexCounter++;
                checkCounter++;
            }
        }
        // if the character is >
        if(inputfile[a] == '>') {
            a++;
            if(inputfile[a] == '=') {
                strcpy(list[lexCounter].lexeme, ">=");
                list[lexCounter].token = 14;
                lexCounter++;
                checkCounter++;
            }
            else {
                strcpy(list[lexCounter].lexeme, ">");
                list[lexCounter].token = 13;
                lexCounter++;
                checkCounter++;
            }
        }
        // if the character is : , showing an invalid symbol error if not accompanied by =
        if(inputfile[a] == ':') {
            a++;
            if(inputfile[a] == '=') {
                strcpy(list[lexCounter].lexeme, ":=");
                list[lexCounter].token = 20;
                lexCounter++;
                checkCounter++;
            }
            else {
                printf("\nError: Invalid symbol error");
                exit(0);
            }
        }
        // if the character is an / and deals with any comments 
        if(inputfile[a] == '/') {
            a++;
            if(inputfile[a] == '*') {
                a++;
                while(a != size) {
                    if(inputfile[a] == '*') {
                        a++;
                        if(inputfile[a] == '/') break;
                        else continue;
                    }
                a++;
                }
            }  
            else {
                strcpy(list[lexCounter].lexeme, "/");
                list[lexCounter].token = 7;
                lexCounter++;
                checkCounter++;
            }
        }
        // if the character is invalid, show an invalid symbol error
        if(checkCounter == 0 && isspace(inputfile[a]) && iscntrl(inputfile[a])) {
            printf("\nError: Invalid symbol error");
            exit(0);
        }
        else if(inputfile[a] == '`' || inputfile[a] == '!' || inputfile[a] == '@' || inputfile[a] == '#' || 
        inputfile[a] == '$' || inputfile[a] == '%' || inputfile[a] == '^' || inputfile[a] == '&' || 
        inputfile[a] == '~' || inputfile[a] == '?' || inputfile[a] == '"' || 
        inputfile[a] == '[' || inputfile[a] == ']' || inputfile[a] == '{' || inputfile[a] == '}') {
            printf("\nError: Invalid symbol error");
            exit(0);
        }

    cacheCounter = 0;
    checkCounter = 0;
    }

    lexeme *newList = malloc(lexCounter * sizeof(lexeme));
    for(int i = 0; i < lexCounter; i++)
        newList[i] = list[i];
  
  if(lCheck == 1) {
  printf("\nLexeme Table: \n");
    printf("lexeme\t\ttoken type\n");
    for(j = 0; j < lexCounter; j++) {
        if(newList[j].token == 3) {
            printf("%d\t\t%d\n", newList[j].number, newList[j].token);
        }
        else if(newList[j].token < 0) {
        }
        else {
            printf("%s\t\t%d\n", newList[j].lexeme, newList[j].token);
        }
    }
  
  
  
	printf("\nLexeme List: \n");
    for(int j = 0;j < lexCounter; j++) {
        if(newList[j].token == 3) {
            printf("%d %d ", newList[j].token, newList[j].number);
        }
        else if(list[j].token == 2){
            printf("%d %s ", newList[j].token, newList[j].lexeme);
        }
        else if(newList[j].token < 0) {
        }
        else {
            printf("%d ", newList[j].token);
        }
    } 
    printf("\n");
  }

	return newList;
}

// identifies the token of each reserved word
int identifierFinder(char* lexeme) {
    if(strcmp(lexeme, "const") == 0) return 28;
    else if(strcmp(lexeme, "var") == 0) return 29;
    else if(strcmp(lexeme, "procedure") == 0) return 30;
    else if(strcmp(lexeme, "call") == 0) return 27;
    else if(strcmp(lexeme, "begin") == 0) return 21;
    else if(strcmp(lexeme, "end") == 0) return 22;
    else if(strcmp(lexeme, "if") == 0) return 23;
    else if(strcmp(lexeme, "then") == 0) return 24;
    else if(strcmp(lexeme, "else") == 0) return 33;
    else if(strcmp(lexeme, "while") == 0) return 25;
    else if(strcmp(lexeme, "do") == 0) return 26;
    else if(strcmp(lexeme, "read") == 0) return 32;
    else if(strcmp(lexeme, "write") == 0) return 31;
    else if(strcmp(lexeme, "odd") == 0) return 8;
    else return 2;
}

// converts a string of numbers to an int
int stringToInt(char* convertString) {
    int convertedNum = 0;
    for (int i = 0; convertString[i] != '\0'; i++) {
        convertedNum = convertedNum * 10 + convertString[i] - '0';
    }
    return convertedNum;
}
