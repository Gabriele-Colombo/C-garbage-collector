#include "Parser.h"
#include <stdio.h>
#include <ctype.h>

#define MAX_LEN 256

char* yytext = "";
int yyleng = 0;
int yyline = 0;

int parser(FILE* file){
    
    static char input_buff[MAX_LEN];                                // Buffer for the line (lexeme)
    char* current;

    current = yytext + yyleng;                                      // Current position on the line

    while(1){
        
        while(*current == '\0'){                                    // !*current

            current = input_buff;                                   // Reset current to initial posizion of the buffer
            if(!fgets(current, MAX_LEN, file)){                     // Take the next line
                *current = '\0';
                return EOI;
            }
            yyline++;

            while(isblank(*current)){
                current++;
            }
        }

        for(; *current; ++current){                                 // Actual tokenizazion of the taken line

            yytext = current;
            yyleng = 1;

            switch(*current){

                case ';': return SEMI;
                case '(': return LP;
                case ')': return RP;
                case '+': return PLUS;
                case '*': return TIMES;

                case ' ':
                case '\t':
                case '\n':
                    break;

                default:
                    if(isalnum(*current)){

                        while(isalnum(*current)){
                            current++;
                        }
                        yyleng = current-yytext;
                        return NUM_OR_ID;

                    }else{
                        printf("Errore %c carattere non riconosciuto\n", current);
                    }
                    break;
            }
        }
    }
}

void main(){

    FILE* file = fopen("Test.txt", "r");
    
    printf("%d\n", parser(file));
    printf("%d\n", parser(file));
    printf("%d\n", parser(file));

    fclose(file);
}