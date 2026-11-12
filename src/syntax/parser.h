#ifndef PARSER_H
#define PARSER_H

#include "coms/common.h"

NodeList parse_assignment(Token *tokens, int *token_cnt, NodeList *mem);

NodeList stack(List *tokens);

#endif