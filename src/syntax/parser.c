#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#include "parser.h"
#include "err/rerr.h"
#include "coms/common.h"

// ==================NOTICE==================
// i dont usually use comments, but i am here 
// because its a little more complicated,
// especially given that im not used to C
// ==========================================

const char *_type_names[] = {
    "LPAREN", "RPAREN", "LBRACE", "RBRACE",
    "COMMA", "DECIMAL", "COLON", "SCOLON", 
    "ADD", "SUB", "STAR", "DIV", "EQUAL", 
    "POWER", "GTHAN", "LTHAN",
    
    "AND", "OR", "NOT", "GOTO", "UNTIL", "IF", "ELSE",
    
    "NUMBER", "STRING",
    
    "EQUIVALENT", "GTHAN_OR_EQUAL_TO", "LTHAN_OR_EQUAL_TO",
    "NOT_EQUAL_TO",
    
    "IDENTIFIER", "COMMENT", "PROGRAM", "MEMORY"

};

const char *_utypes[] = {
    "NODE", "NODES"
};

void _strcpy(NodeMaster *node, char *stor)
{
    int strln = strlen(stor) + 1;
    
    node->UNION.Node.stor = malloc(strln);
    strcpy(node->UNION.Node.stor, stor);
}

void umem(NodeMaster *mem, NodeMaster x, size_t *memsz)
{
    *memsz += sizeof(x);
    mem->UNION.Nodes.nodes = realloc(mem->UNION.Nodes.nodes, *memsz);
    mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln] = x;
    printf("added type %s with stor %s into memory at index %d\n", _utypes[x.utype], x.UNION.Node.stor, mem->UNION.Nodes.nodeln);
    mem->UNION.Nodes.nodeln++;
}

void amem(NodeMaster *mem, size_t *memsz, Token *tokens, int *token_cnt)
{
    Token token = tokens[*token_cnt];
    
    NodeMaster tnode;
    tnode.utype = NODE;
    _strcpy(&tnode, token.stor);
    tnode.UNION.Node.type = token.type;
    
    umem(mem, tnode, memsz);
}


NodeMaster parse_expr(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
{
    Token token = tokens[*token_cnt];
    
    // mstack --> ministack, like a branch in a tree 
    size_t mstacksz = 0;
    NodeMaster mstack;
    mstack.utype = NODES;
    mstack.UNION.Nodes.nodes = NULL;
    mstack.UNION.Nodes.nodeln = 0;
    
    while (true)
        {
            //printf("-> %s\n", token.stor);
            if (token.type == RPAREN || token.type == SCOLON) 
                {
                    (*token_cnt)++;
                    return mstack; 
                }
            else if (token.type == LPAREN)
                {
                    (*token_cnt)++;
                    
                    NodeMaster tmp = parse_expr(tokens, token_cnt, mem, memsz);
                    umem(mem, tmp, memsz);
                }
            
            else if (token.type == ADD || token.type == SUB || token.type == STAR || token.type == DIV)
                {
                    // this is a mini-mini stack --> [OP][x][y]                
                    size_t mmstacksz = 0;
                    NodeMaster mmstack;
                    mmstack.utype = NODES;
                    mmstack.UNION.Nodes.nodes = NULL;
                    mmstack.UNION.Nodes.nodeln = 0;
                    
                    // fetch the previous token from memory
                    NodeMaster prev_token = mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1];
                    
                    // assign the first value (the terminal index) to the assignment token [ ADD || SUB || STAR || DIV ]      
                    NodeMaster term;
                    term.utype = NODE;
                    term.UNION.Node.type = token.type;
                    _strcpy(&term, token.stor);
                    
                    // copy it into mini-mini stack
                    mmstacksz += sizeof(term);
                    mmstack.UNION.Nodes.nodes = realloc(mmstack.UNION.Nodes.nodes, mmstacksz);
                    mmstack.UNION.Nodes.nodes[mmstack.UNION.Nodes.nodeln] = term;
                    mmstack.UNION.Nodes.nodeln++;
                    
                    // copy the next two values into the mini stack
                    //     left token (which is in mem) 
                    mmstacksz += sizeof(prev_token);
                    mmstack.UNION.Nodes.nodes = realloc(mmstack.UNION.Nodes.nodes, mmstacksz);
                    mmstack.UNION.Nodes.nodes[mmstack.UNION.Nodes.nodeln] = prev_token;
                    mmstack.UNION.Nodes.nodeln++;
                    
                    //    right token
                    NodeMaster next_token;
                    if (tokens[(*token_cnt) + 1].type == LPAREN)
                        {
                            (*token_cnt) += 2;
                            next_token = parse_expr(tokens, token_cnt, mem, memsz);
                        }
                    else
                        {
  
                            next_token.utype = NODE;
                            next_token.UNION.Node.type = tokens[(*token_cnt) + 1].type;
                            _strcpy(&next_token, tokens[(*token_cnt) + 1].stor);
                            
                            (*token_cnt)++;
                        }
                    
                    mmstacksz += sizeof(next_token);
                    mmstack.UNION.Nodes.nodes = realloc(mmstack.UNION.Nodes.nodes, mmstacksz);
                    mmstack.UNION.Nodes.nodes[mmstack.UNION.Nodes.nodeln] = next_token;
                    mmstack.UNION.Nodes.nodeln++;
                    
                    // update the memory
                    umem(mem, mmstack, memsz);
                    
                    // update mstack with mmstack       
                    mstacksz += sizeof(mmstack);
                    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                    mstack.UNION.Nodes.nodeln++;
                    
                }
            else
                {
                    amem(mem, memsz, tokens, token_cnt);
                }
                
            (*token_cnt)++;
            token = tokens[*token_cnt];
        }
}

NodeMaster parse_assignment(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
{    
    //printf("UTYPE %s\n", _utypes[mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1].utype]);
    
    NodeMaster token = mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1];
    Token next_token = tokens[(*token_cnt) + 1];
    
    // mstack --> ministack, like a branch in a tree   
    size_t mstacksz = 0;
    NodeMaster mstack;
    mstack.utype = NODES;
    mstack.UNION.Nodes.nodes = NULL;
    mstack.UNION.Nodes.nodeln = 0;
    
    // assign the first value (the terminal index) to the assignment token [EQUAL]
    NodeMaster assig;
    assig.utype = NODE;
    assig.UNION.Node.type = next_token.type;
    _strcpy(&assig, next_token.stor);
    
    // copy that value into the stack (actually placing it into the stack)
    mstacksz += sizeof(assig);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = assig;
    mstack.UNION.Nodes.nodeln++;

    // append variable [IDENTIFIER] to be assigned
    mstacksz += sizeof(token);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = token;
    mstack.UNION.Nodes.nodeln++;
    
    // find the assignment value  
    *token_cnt += 2;
    NodeMaster value = parse_expr(tokens, token_cnt, mem, memsz);
    
    // append value into the stack   
    mstacksz += sizeof(value);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = value;
    mstack.UNION.Nodes.nodeln++;
    
    return mstack;
}


NodeMaster stack(List *tokens)
{
    Token *_tokens = (Token *)tokens->element;
    int token_cnt = 0;
    int *ptoken_cnt = &token_cnt;
    
    // initialize the entire stack (PROGRAM refers to an entire file) 
    size_t stacksz = 0;
    NodeMaster _stack;
    _stack.utype = NODES;
    _stack.UNION.Nodes.type = PROGRAM;
    _stack.UNION.Nodes.nodes = NULL;
    _stack.UNION.Nodes.nodeln = 0;
    
    // initialize the entire memory stack
    size_t memsz = 0;
    NodeMaster mem;
    mem.utype = NODES;
    mem.UNION.Nodes.type = MEMORY;
    mem.UNION.Nodes.nodes = NULL;
    mem.UNION.Nodes.nodeln = 0;
    
    while (token_cnt < tokens->length - 1)
        {
            Token token = _tokens[token_cnt];
            Token next_token = _tokens[token_cnt + 1];
            
            // checking if any are terminal
            const enum _TYPE_ TOKEN_TYPE = token.type;
            const enum _TYPE_ NEXT_TOKEN_TYPE = next_token.type;
            
            // always update memory with each token
            //    we first create a Node for the current token
            amem(&mem, &memsz, _tokens, ptoken_cnt);
            
            switch (TOKEN_TYPE)
            {
                case IDENTIFIER:  
                    if (NEXT_TOKEN_TYPE == LPAREN)
                        {
                            // parse_function
                        }
                    else if (NEXT_TOKEN_TYPE == EQUAL)
                        {
                            NodeMaster mstack = parse_assignment(_tokens, ptoken_cnt, &mem, &memsz);
                            // append it to the stack       
                            stacksz += sizeof(mstack);
                            _stack.UNION.Nodes.nodes = realloc(_stack.UNION.Nodes.nodes, stacksz);
                            _stack.UNION.Nodes.nodes[ _stack.UNION.Nodes.nodeln] = mstack;
                            _stack.UNION.Nodes.nodeln++;
                            
                        }
                    else if (NEXT_TOKEN_TYPE == DECIMAL)
                        {
                            // parse_IDlinker
                        }
                    else if (NEXT_TOKEN_TYPE == ADD || NEXT_TOKEN_TYPE == SUB || NEXT_TOKEN_TYPE == STAR || NEXT_TOKEN_TYPE == DIV)
                        {
                            //NodeList mstack = parse_expr(_tokens, ptoken_cnt, &mem);
                            
                            NodeMaster mstack = parse_expr(_tokens, ptoken_cnt, &mem, &memsz);
                            // append it to the stack       
                            stacksz += sizeof(mstack);
                            _stack.UNION.Nodes.nodes = realloc(_stack.UNION.Nodes.nodes, stacksz);
                            _stack.UNION.Nodes.nodes[_stack.UNION.Nodes.nodeln - 1] = mstack;
                            _stack.UNION.Nodes.nodeln++;
                        }
                    break;
                case LPAREN:
                    // parse_expr
                    break;
                case LBRACE:
                    // parse_scope
                    break;
                    
                case EQUAL:
                    // prase_expr
                    break;
            }
        }
        
    return _stack;
}