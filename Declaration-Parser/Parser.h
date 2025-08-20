#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdbool.h>
#include "LexicalAnalyzer.h"

void declaration();
void name();
void type();
void mod();
bool sign();
void len(bool* anyLen, bool* onlyLong);

#endif