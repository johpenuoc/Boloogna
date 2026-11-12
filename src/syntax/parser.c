#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "coms/common.h"

// ADD: "Expr | ADD | Expr"
// IF: IF | Expr | *code*
// !: ! | Expr

NodeList parse_assignment(Token *tokens, int *token_cnt, NodeList *mem)
{
    NodeList token = mem->Nodelst->nodelst[mem->Nodelst.nodelst.nodeln];
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
                            // prase_expr
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