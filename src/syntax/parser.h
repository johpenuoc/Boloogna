#ifndef PARSER_H
#define PARSER_H

#include "coms/common.h"

NodeList parse_factor(Token *tokens, int *token_cnt, NodeList *mem);
NodeList parse_term(Token *tokens, int *token_cnt, NodeList *mem);
NodeList parse_expr(Token *tokens, int *token_cnt, Nodelist *mem);
NodeList parse_assignment(Token *tokens, int *token_cnt, NodeList *mem);

NodeList stack(List *tokens);

#endif