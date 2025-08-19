#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "LexicalAnalyzer.h"
#include "Var.h"

#define MAX_ARGS 16

void statement();
void expression(char* var);
void term(char* var);
void factor(char* var);
bool legal_lookahead(int first_args, ...);

void statement(){

    /*
        statements -> expression; EOI
        statements -> expression; statement
    */

    char* var;

    while(!match(EOI)){

        expression(var = getVar());
        freeVar(var);
        puts("");

        if(match(SEMI)){
            advance();
        }else{
            fprintf(stderr, "%d, Error: Missing semicolon on line\n", yyline);
        }
    }
}

void expression(char* var){

    /*
        expression -> term expression'
        expression' -> + term expression' | E

    */

    if(!legal_lookahead(NUM_OR_ID, LP, 0 ) )
        return;
    
    term(var);

    char* var2;
    while(match(PLUS)){
        advance();
        term(var2 = getVar());
        printf("%s += %s\n", var, var2);
        freeVar(var2);
    }
}

void term(char* var){

    /*
        term -> factor term'
        term' -> * factor term' | E

    */
    if(!legal_lookahead(NUM_OR_ID, LP, 0 ) )
        return;

    factor(var);

    char* var2;

    while(match(TIMES)){
        advance();
        factor(var2 = getVar());
        printf("%s *= %s\n", var, var2);
        freeVar(var2);
    }
}

void factor(char* var){

    /*
        factor -> NUM_OR_ID
        factor -> (expression)
    */
    if(!legal_lookahead(NUM_OR_ID, LP, 0 ) )
        return;

    if(match(NUM_OR_ID)){
        printf("%s = %0.*s\n", var, yyleng, yytext);
        advance();

    }else if((match(LP))){

        advance();
        expression(var);
        if(match(RP)){
            advance();
        }else{
            fprintf(stderr, "%d, Error: Parenthesis doesn't match\n", yyline);
        }

    }else{
        fprintf(stderr, "%d, Error: Number or id missing\n", yyline);
    }
}

bool legal_lookahead(int first_arg, ...){

    /*
    * Simple error detection and recovery. Arguments are a 0-terminated list of
    * those tokens that can legitimately come next in the input. If the list is
    * empty, the end of file must come next. Print an error message if
    * necessary. Error recovery is performed by discarding all input symbols
    * until one that's in the input list is found
    *
    * Return true if there's no error or if we recovered from the error,
    * false if we can't recover.
    */

    va_list args;
    int* current = NULL;
    int lookAHeads[MAX_ARGS] = {0};
    int* p = lookAHeads;
    int tok;

    bool res;
    bool error = false;

    va_start(args, first_arg);

    if(!first_arg){
        if(match(EOI)){
            res=true;
        }
    }else{

        *p++ = first_arg;
        while( (tok = va_arg(args, int)) && p < &lookAHeads[MAX_ARGS] ){
            *p++ = tok;
        }

        while(!match(SEMI)){

            for(current=lookAHeads; current<p; current++){

                if(match(*current)){
                    res = true;
                    goto end;
                }
            }
            if(!error){
                fprintf(stderr, "%d, Error: No first set value\n", yyline);
                error = true;
            }

            advance();      // discarding all input symbols until one that's in the input list is found
        }
    }
    end:
        va_end(args);
        return res;
}
