#ifndef SCAN_H
#define SCAN_H

#include <stdio.h>

#include "coms/common.h"


List preprocessor(FILE *file);
void incr(char *curr, int *len, char indv);
void shift_down(Token *tokens, int start, int jump, int tokens_length);
int combine_chars(Token *tokens, int token_cnt, int i);
List tokenize(List l);

#endif