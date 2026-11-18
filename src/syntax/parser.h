#ifndef PARSER_H
#define PARSER_H

#include "coms/common.h"


void sto_mstack(NodeMaster *mstack, size_t *mstacksz, NodeMaster node_s);
void umem(NodeMaster *mem, NodeMaster x, size_t *memsz);
void amem(NodeMaster *mem, size_t *memsz, Token *tokens, int *token_cnt);

NodeMaster parse_expr(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);
NodeMaster _parse_condition(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);
NodeMaster parse_assignment(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);
NodeMaster parse_func(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);
NodeMaster parse_IDlinker(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);
NodeMaster parse_if(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);
NodeMaster parse_scope(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);

NodeMaster stack(List *tokens);

#endif