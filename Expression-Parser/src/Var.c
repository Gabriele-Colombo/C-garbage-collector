#include <stdio.h>
#include <stdlib.h>
#include "LexicalAnalyzer.h"

char* variables[] = {"V1", "V2", "V3", "V4", "V5", "V6", "V7"};
char** pos = variables;

char* getVar(){

    // Address in 'pos' is greater then the address of the last element, '&variables[6])'
    if(pos >= &variables[sizeof(variables)/sizeof(*variables)]){
        fprintf(stderr, "%d, Error: Too much variables needed\n", yyline);
        exit(1);
    }
    return (*pos++);
}

void freeVar(char* s){

    if(pos > variables){
        *--pos = s;
    }else{
        fprintf(stderr, "%d, Error: Stack underflow\n", yyline);
    }
}