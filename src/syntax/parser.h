#ifndef PARSER_H
#define PARSER_H

#include "coms/common.h"

//NodeList parse_factor(Token *tokens, int *token_cnt, NodeList *mem);
//NodeList parse_term(Token *tokens, int *token_cnt, NodeList *mem);
void umem(NodeMaster *mem, NodeMaster x, size_t *memsz);
void amem(NodeMaster *mem, size_t *memsz, Token *tokens, int *token_cnt);
NodeMaster parse_expr(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);
NodeMaster parse_assignment(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);

NodeMaster stack(List *tokens);

#endif