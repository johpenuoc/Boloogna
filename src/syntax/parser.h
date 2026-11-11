#ifndef PARSER_H
#define PARSER_H

#include "coms/common.h"

void add(Branch *branch, char *prev_expr, char *next_expr, const enum _TYPE_ curr_type);

List ast(List *tokens);

#endif