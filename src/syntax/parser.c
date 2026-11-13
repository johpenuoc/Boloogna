#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "parser.h"
#include "err/rerr.h"
#include "coms/common.h"

// ==================NOTICE==================
// i dont usually use comments, but i am here 
// because its a little more complicated,
// especially given that im not used to C
//===========================================


NodeList parse_expr(Token *tokens, int *token_cnt, NodeList *mem);


NodeList parse_factor(Token *tokens, int *token_cnt, NodeList *mem)
{
    Token token = tokens[*token_cnt];
    
    // mstack --> ministack, like a branch in a tree
    NodeList mstack;
    mstack.Nodelst.nodelst = NULL;
    mstack.Nodelst.nodeln = 0;
    size_t mstacksz = 0;
    
    switch (token.type)
        {
            case LPAREN:
                (*token_cnt)++;
                NodeList pe = parse_expr(tokens, token_cnt, mem);
                
                mstacksz++;
                mstack.Nodelst.nodelst = realloc(mstack.Nodelst.nodelst, mstacksz);
                mstack.Nodelst.nodelst[mstack.Nodelst.nodeln] = pe;
                mstack.Nodelst.nodeln++;
                break;
            case RPAREN:
                (*token_cnt)++;
                return mstack; 
            
            case IDENTIFIER:
                break;
            case NUMBER:
                break;
            
            default:
                raise_uterr();
                break;
        }
}

NodeList parse_term(Token *tokens, int *token_cnt, NodeList *mem)
{
    NodeList pf = parse_factor(tokens, token_cnt, mem);
    
    while ()
        {
            
        }
}

// x = 4 - 3 * 2;
// y = 2 * 3 - 4;
// if term: make NodeList
// NodeList n, n[0] = *, n[1] = mem[0]

NodeList parse_expr(Token *tokens, int *token_cnt, NodeList *mem)
{
    Token token = tokens[*token_cnt];
    
    // mstack --> ministack, like a branch in a tree
    NodeList mstack;
    mstack.Nodelst.nodelst = NULL;
    mstack.Nodelst.nodeln = 0;
    size_t mstacksz = 0;
    
    while (true)
        {
            NodeList pt = parse_term(tokens, token_cnt, mem);
        }
        
    return mstack;
}

// 2 + 5 - (7 * 4)

NodeList parse_expr(Token *tokens, int *token_cnt, NodeList *mem)
{
    Token token = tokens[*token_cnt];
    
    // mstack --> ministack, like a branch in a tree
    NodeList mstack;
    mstack.Nodelst.nodelst = NULL;
    mstack.Nodelst.nodeln = 0;
    size_t mstacksz = 0;
    
    while (true)
        {
            if (token.type == RPAREN) return mmstack;
            else if (token.type == LPAREN)
                {
                    (*token_cnt)++;
                    mem->Nodels.nodelst[mem->Nodelst.nodeln] = parse_expr(tokens, token_cnt, mem)
                }
            
            if (token.type == ADD || token.type == SUB || token.type == STAR || token.type == DIV)
                {
                    // this is a mini-mini stack --> [OP][x][y]
                    NodeList mmstack;
                    mmstack.Nodelst.nodelst = NULL;
                    mmstack.Nodelst.nodeln = 0;
                    size_t mmstacksz = 0;
                    
                    // fetch the previous token from memory
                    NodeList prev_token = mem->Nodels.nodelst[mem->Nodelst.nodeln];
                    
                    // assign the first value (the terminal index) to the assignment token [ ADD || SUB || STAR || DIV ]
                    NodeList term;
                    term.node.type == token.type;
                    strcpy(term.node.stor, token.stor);
                    
                    // copy it into mini-mini stack
                    mmstacksz += sizeof(term);
                    mmstack.Nodelst.nodelst = realloc(mmstack.Nodelst.nodelst, mmstacksz);
                    mmstack.Nodelst.nodelst[0] = term;
                    
                    // copy the next two values into the mini stack
                    //     left token (which is in mem)
                    mmstacksz += sizeof(prev_token);
                    mmstack.Nodelst.nodelst = realloc(mmstack.Nodelst.nodelst, mmstacksz);
                    mmstack.Nodelst.nodelst[1] = prev_token;
                    
                    //    right token
                    if (tokens[(*token_cnt) + 1] == LPAREN)
                        {
                            (*token_cnt)++;
                            NodeList next_token = parse_expr(tokens, token_cnt, mem)
                        }
                    else
                        {
                            NodeList next_token = tokens[(*token_cnt) + 1];
                        }
                    mmstacksz += sizeof(next_token);
                    mmstack.Nodelst.nodelst = realloc(mmstack.Nodelst.nodelst, mmstacksz);
                    mmstack.Nodelst.nodelst[2] = next_token;
                    
                    // update the memory
                    mem->Nodels.nodelst[mem->Nodelst.nodeln] = mmstacks;
                    
                }
            
            token = tokens[*token_cnt];
            (*token_cnt)++;
        }
}

NodeList parse_assignment(Token *tokens, int *token_cnt, NodeList *mem)
{
    NodeList token = mem->Nodelst.nodelst[mem->Nodelst.nodeln];
    Token next_token = tokens[(*token_cnt) + 1];
    
    // mstack --> ministack, like a branch in a tree
    NodeList mstack;
    mstack.Nodelst.nodelst = NULL;
    mstack.Nodelst.nodeln = 0;
    size_t mstacksz = 0;
    
    // assign the first value (the terminal index) to the assignment token [EQUAL]
    NodeList assig;
    assig.node.type = next_token.type;
    strcpy(assign.node.stor, next_token.stor);
    
    // copy that value into the stack (actually placing it into the stack)
    mstacksz += sizeof(assig);
    mstack.Nodelst.nodelst = realloc(mstack.Nodelst.nodelst, mstacksz);
    mstack.Nodelst.nodelst[0] = assig;
    
    // assign left node as a new object
    /*Token token = tokens[token_cnt];
    NodeList ltoken;
    ltoken.node.type = token.type;
    strcpy(ltoken.node.stor, token.stor);*/
    
    // append ltoken object into the stack
    mstacksz += sizeof(token);
    mstack.Nodelst.nodelst = realloc(mstack.Nodelst.nodelst, mstacksz);
    mstack.Nodelst.nodelst[1] = token;
    
    // find the assignment value
    *token_cnt += 2;
    NodeList value = parse_expr(tokens, token_cnt);
    
    // append value into the stack
    mstacksz += sizeof(value);
    mstack.Nodelst.nodelst = realloc(mstack.Nodelst.nodelst, mstacksz);
    mstack.Nodelst.nodelst[2] = value;
    
    return mstack;
}


NodeList stack(List *tokens)
{
    Token *_tokens = (Token *)tokens->element;
    int token_cnt = 0;
    int *ptoken_cnt = &token_cnt;
    
    // initialize the entire stack (PROGRAM refers to an entire file)
    size_t stacksz = 1;
    NodeList *stack = malloc(sizeof(NodeList) * stacksz);
    stack[0].type = PROGRAM;
    stack[0].Nodelst.nodelst = NULL;
    stack[0].Nodelst.nodeln = 0;
    
    // initialize the entire memory stack
    size_t memsz = 1;
    NodeList *mem = malloc(sizeof(NodeList) * memsz);
    /*mem[0].type = PROGRAM;
    mem[0].Nodelst.nodelst = NULL;
    mem[0].Nodelst.nodeln = 0;*/
    
    while (token_cnt < tokens->length - 1)
        {
            Token token = _tokens[token_cnt];
            Token next_token = _tokens[token_cnt + 1];
            
            // checking if any are terminal
            const enum _TYPE_ TOKEN_TYPE = token.type;
            const enum _TYPE_ NEXT_TOKEN_TYPE = next_token.type;
            switch (TOKEN_TYPE)
            {
                case IDENTIFIER:
                    if (NEXT_TOKEN_TYPE == LPAREN)
                        {
                            // parse_function
                        }
                    else if (NEXT_TOKEN_TYPE == EQUAL)
                        {
                            NodeList mstack = parse_assignment(tokens, ptoken_cnt, &mem);
                            
                        }
                    else if (NEXT_TOKEN_TYPE == DECIMAL)
                        {
                            // parse_IDlinker
                        }
                    else if (NEXT_TOKEN_TYPE == ADD || NEXT_TOKEN_TYPE == SUB || NEXT_TOKEN_TYPE == STAR || NEXT_TOKEN_TYPE == DIV)
                        {
                            NodeList mstack = parse_expr(tokens, ptoken_cnt, &mem);
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
        
    return stack;
}