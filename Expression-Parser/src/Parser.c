#include <stdio.h>
#include "LexicalAnalyzer.h"

void Ostatement();
void Oexpression();
void expressionPrime();
void Oterm();
void termPrime();
void Ofactor();

void Ostatement(){

    /*
        statements -> expression; EOI
        statements -> expression; statement
    */

    Oexpression();

    if(match(SEMI)){
        advance();
    }else{
        fprintf(stderr, "%d, Error: Missing semicolon on line\n", yyline);
    }

    if(!match(EOI)){
        Ostatement();
    }
}

void Oexpression(){

    /*
        expression -> term expression'
    */
    
    Oterm();
    expressionPrime();
}

void expressionPrime(){

    /*
        expression' -> + term expression' | E
    */

    if(match(PLUS)){
        advance();
        Oterm();
        expressionPrime();
    }
}

void Oterm(){

    /*
        term -> factor term'
    */

    Ofactor();
    termPrime();
}

void termPrime(){

    /*
        term' -> * factor term' | E
    */

    if(match(TIMES)){
        advance();
        Ofactor();
        termPrime();
    }
}

void Ofactor(){

    /*
        factor -> NUM_OR_ID
        factor -> (expression)
    */

    if(match(NUM_OR_ID)){
        advance();

    }else if((match(LP))){

        advance();
        Oexpression();

        if(match(RP)){
            advance();
        }else{
            fprintf(stderr, "%d, Error: Parenthesis doesn't match\n", yyline);
        }

    }else{
        fprintf(stderr, "%d, Error: Number or id missing\n", yyline);
    }
}
