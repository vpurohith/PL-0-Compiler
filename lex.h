#ifndef __LEX_H
#define __LEX_H

typedef struct lexeme
{
	char lexeme[12];
    int number;
    int token;
} lexeme;


lexeme* lex_analyze(char *inputfile, int size, int lCheck);

#endif