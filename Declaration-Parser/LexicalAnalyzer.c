#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "LexicalAnalyzer.h"

#define MAX_COUNT 256

typedef struct{
    const char* word;
    int token;
}Token;

int yyline = 0;
int yyleng = 0;
char* yytext = ""; 

int lexicalAnalyzer(){

    static Token tokens[] = {
        {"int",INT},
        {"char",CHAR},
        {"float",FLOAT},
        {"double",DOUBLE},
        {"signed",SIGNED},
        {"unsigned",UNSIGNED},
        {"long",LONG},
        {"short",SHORT},
        {"const",CONST},
        {"volatile",VOLATILE},
        {NULL, 0}
    };

    static FILE* file = NULL;
    if(!file) file=fopen("Declaration.txt", "r");

    static char buffer[MAX_COUNT];
    char* current = yytext + yyleng;

    while(1){

        while(*current == '\0'){

            current = buffer;
            if(!fgets(buffer, MAX_COUNT, file)){
                *current='\0';
                return EOI;
            }

            ++yyline;

            while(isblank(*current)){
                current++;
            }
        }

        for(; *current; current++){

            yytext = current;
            yyleng = 1;
            
            switch(*current){

                case ';':
                    return SEMI;

                case '\n':
                case '\t':
                case ' ':
                    break;

                default:
                    char next[256] = {0};
                    size_t len = 0;

                    while(isalnum(*current) || *current=='_'){
                        next[len++] = *current++;
                    }
                    yyleng = len;
                    next[len+1]='\0';

                    for(size_t i=0; tokens[i].word != NULL; i++){
                        if(strcmp(tokens[i].word, next)==0){
                            return tokens[i].token;
                        }
                    }
                    return NUM_ID;
            }
        }
    }
}

int lookahead = -1;

bool match(int token){

    if(lookahead == -1){
        lookahead = lexicalAnalyzer();
    }
    return token == lookahead;
}

bool advance(){
    lookahead = lexicalAnalyzer();
}