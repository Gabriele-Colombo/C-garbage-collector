#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#define INT         1
#define CHAR        2
#define FLOAT       3
#define DOUBLE      4
#define SIGNED      5
#define UNSIGNED    6
#define LONG        7
#define SHORT       8
#define CONST       9
#define VOLATILE    10
#define EOI         11
#define SEMI        12
#define NUM_ID      13

extern int yyline;
extern char* yytext;
extern int yyleng;

int lexicalAnalyzer();
bool match(int token);
bool advance();

#endif