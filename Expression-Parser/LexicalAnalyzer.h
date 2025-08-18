#define EOI 0           // end of input
#define PLUS 1          // +
#define TIMES 2         // *
#define LP 3            // (
#define RP 4            // )
#define SEMI 5          // ;
#define NUM_OR_ID 6     // other

extern char* yytext;
extern int yyposition;
extern int yyline;