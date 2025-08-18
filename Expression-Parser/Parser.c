#include <stdio.h>
#include "LexicalAnalyzer.h"
#include "LexicalAnalyzer.c"

void statement(FILE* file);
void expression(FILE* file);
void expressionPrime(FILE* file);
void term(FILE* file);
void termPrime(FILE* file);
void factor(FILE* file);

void main(){

    FILE* file = fopen("Test.txt", "r");
    statement(file);
    fclose(file);
}

void statement(FILE* file){

    /*
        statements -> expression; EOI
        statements -> expression; statement
    */

    expression(file);

    if(match(file, SEMI)){
        advance(file);
    }else{
        printf("Error missing semicolon on line %d\n", yyline);
    }

    if(!match(file, EOI)){
        statement(file);
    }
}

void expression(FILE* file){

    /*
        expression -> term expression'
    */
    
    term(file);
    expressionPrime(file);
}

void expressionPrime(FILE* file){

    /*
        expression' -> + term expression' | E
    */

    if(match(file, PLUS)){
        advance(file);
        term(file);
        expressionPrime(file);
    }
}

void term(FILE* file){

    /*
        term -> factor term'
    */

    factor(file);
    termPrime(file);
}

void termPrime(FILE* file){

    /*
        term' -> * factor term' | E
    */

    if(match(file, TIMES)){
        advance(file);
        factor(file);
        termPrime(file);
    }
}

void factor(FILE* file){

    /*
        factor -> NUM_OR_ID
        factor -> (expression)
    */

    if(match(file, NUM_OR_ID)){
        advance(file);

    }else if((match(file, LP))){

        advance(file);
        expression(file);

        if(match(file, RP)){
            advance(file);
        }else{
            printf("Error parenthesis on line %d\n", yyline);
        }

    }else{
        printf("Error number or id missing on line %d\n", yyline);
    }
}