#ifndef SCAN_H
#define SCAN_H

#include <stdio.h>

#include "coms/common.h"


List preprocessor(FILE *file);
void incr(char *curr, int *len, char indv);
void shift_down(Token *tokens, int start, int jump, int tokens_length);
void is_float(Token *tokens, int *token_cnt);
void combine_tokens(Token *tokens, int *token_cnt, enum _TYPE_ T1, enum _TYPE_ T2, enum _TYPE_ MT);
void separate_tokens(Token *tokens, int *token_cnt, enum _TYPE_ MT, enum _TYPE_ T2);
List tokenize(FILE *file);

#endif