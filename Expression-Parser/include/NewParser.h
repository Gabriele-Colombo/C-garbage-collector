#include <stdbool.h>

#ifndef NEWPARSER_H
#define NEWPARSER_H

void statement();
void expression(char* var);
void term(char* var);
void factor(char* var);
bool legal_lookahead(int first_args, ...);

#endif