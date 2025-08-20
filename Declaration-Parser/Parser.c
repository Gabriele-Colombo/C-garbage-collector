#include <stdio.h>
#include <stdbool.h>
#include "LexicalAnalyzer.h"

void declaration();
void name();
void type();
void mod();
bool sign();
void len(bool* anyLen, bool* onlyLong);

void declaration(){

    while(!match(EOI)){

        type();
        name();

        if(!match(SEMI)){
            fprintf(stderr, "%d, Error: missing semicolon\n", yyline);
        }else{
            advance();
        }
    }
}

void name(){

    if(!match(NUM_ID)){
        fprintf(stderr, "%d, Error: missing variable NAME\n", yyline);
    }else{
        advance();
    }
}

void type(){

    mod();

    bool s = sign();
    bool anyLen = false;
    bool onlyLong = false;
    len(&anyLen, &onlyLong);

    if(s){
        if(!match(INT) && !match(CHAR)){
            fprintf(stderr, "%d, Error: expected INT or CHAR\n", yyline);
        }
        advance();
        return;
    }

    if(anyLen){
        if(onlyLong){
            if(!match(INT) && !match(DOUBLE)){
                fprintf(stderr, "%d, Error: expected INT or DOUBLE\n", yyline);
            }else{
                advance();
            }
        }else{
            if(!match(INT)){
                fprintf(stderr, "%d, Error: expected INT\n", yyline);
            }else{
                advance();
            }
        }
        return;
    }

    if(match(INT) || match(CHAR) || match(FLOAT) || match(DOUBLE)){
        advance();
    } else {
        fprintf(stderr, "%d, Error: expected type specifier\n", yyline);
    }
}

void mod(){
    if(match(CONST) || match(VOLATILE)){
        advance();
        mod();
    }
}

bool sign(){

    if(match(SIGNED) || match(UNSIGNED)){
        advance();
        return true;
    }
    return false;
}

void len(bool* anyLen, bool* onlyLong){

    if(match(SHORT)){
        advance();
        *anyLen = true;
        return;
    }

    if(match(LONG)){
        advance();
        *anyLen = true;
        *onlyLong = true;

        if(match(LONG)){
            advance();
            *onlyLong = false;
        }
    }

}