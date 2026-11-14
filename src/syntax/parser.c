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
// ==========================================

void umem(NodeMaster *mem, NodeMaster *x, size_t *memsz)
{
    *memsz += sizeof(*x);
    mem->UNION.Nodes.nodes = realloc(mem->UNION.Nodes.nodes, *memsz);
    mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln] = *x;
    mem->UNION.Nodes.nodeln++;
}


NodeMaster parse_expr(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
{
    Token token = tokens[*token_cnt];
    
    // mstack --> ministack, like a branch in a tree
    /*NodeList mstack;
    mstack.UNode.Nodelst.nodelst = NULL;
    mstack.UNode.Nodelst.nodeln = 0;
    size_t mstacksz = 0;*/
    
    size_t mstacksz = 0;
    NodeMaster mstack;
    mstack.utype = NODES;
    mstack.UNION.Nodes.nodes = NULL;
    mstack.UNION.Nodes.nodeln = 0;
    
    while (true)
        {
            if (token.type == RPAREN || token.type == SCOLON) 
                {
                    (*token_cnt)++;
                    return mstack; 
                }
            else if (token.type == LPAREN)
                {
                    (*token_cnt)++;
                    
                    /*mem->UNode.Nodelst.nodelst[mem->UNode.Nodelst.nodeln] = parse_expr(tokens, token_cnt, mem);*/
                    
                    NodeMaster tmp = parse_expr(tokens, token_cnt, mem, memsz);
                    umem(mem, &tmp, memsz);
                }
            
            else if (token.type == ADD || token.type == SUB || token.type == STAR || token.type == DIV)
                {
                    // this is a mini-mini stack --> [OP][x][y]
                    /*NodeList mmstack;
                    mmstack.UNode.Nodelst.nodelst = NULL;
                    mmstack.UNode.Nodelst.nodeln = 0;
                    size_t mmstacksz = 0;*/
                    
                    size_t mmstacksz = 0;
                    NodeMaster mmstack;
                    mmstack.utype = NODES;
                    mmstack.UNION.Nodes.nodes = NULL;
                    mmstack.UNION.Nodes.nodeln = 0;
                    
                    // fetch the previous token from memory
                    /*NodeList prev_token = mem->UNode.Nodelst.nodelst[mem->UNode.Nodelst.nodeln];*/
                    
                    NodeMaster prev_token = mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1];
                    
                    // assign the first value (the terminal index) to the assignment token [ ADD || SUB || STAR || DIV ]
                    /*NodeList term;
                    term.node.type = token.type;
                    strcpy(term.node.stor, token.stor);*/
                    
                    NodeMaster term;
                    term.utype = NODE;
                    term.UNION.Node.type = token.type;
                    strcpy(term.UNION.Node.stor, token.stor);
                    
                    // copy it into mini-mini stack
                    /*mmstacksz += sizeof(term);
                    mmstack.UNode.Nodelst.nodelst = realloc(mmstack.UNode.Nodelst.nodelst, mmstacksz);
                    mmstack.UNode.Nodelst.nodelst[0] = term;*/
                    
                    mmstacksz += sizeof(term);
                    mmstack.UNION.Nodes.nodes = realloc(mmstack.UNION.Nodes.nodes, mmstacksz);
                    mmstack.UNION.Nodes.nodes[mmstack.UNION.Nodes.nodeln - 1] = term;
                    mmstack.UNION.Nodes.nodeln++;
                    
                    // copy the next two values into the mini stack
                    //     left token (which is in mem)
                    /*mmstacksz += sizeof(prev_token);
                    mmstack.UNode.Nodelst.nodelst = realloc(mmstack.UNode.Nodelst.nodelst, mmstacksz);
                    mmstack.UNode.Nodelst.nodelst[1] = prev_token;*/
                    
                    mmstacksz += sizeof(prev_token);
                    mmstack.UNION.Nodes.nodes = realloc(mmstack.UNION.Nodes.nodes, mmstacksz);
                    mmstack.UNION.Nodes.nodes[mmstack.UNION.Nodes.nodeln - 1] = prev_token;
                    mmstack.UNION.Nodes.nodeln++;
                    
                    //    right token
                    //NodeList next_token;
                    NodeMaster next_token;
                    if (tokens[(*token_cnt) + 1].type == LPAREN)
                        {
                            (*token_cnt) += 2;
                            next_token = parse_expr(tokens, token_cnt, mem, memsz);
                        }
                    else
                        {
                            /*next_token.type = tokens[(*token_cnt) + 1].type;
                            strcpy(next_token.node.stor, tokens[(*token_cnt) + 1].stor);
                            (*token_cnt)++;*/
                            
                            next_token.utype = NODE;
                            next_token.UNION.Node.type = tokens[(*token_cnt) + 1].type;
                            strcpy(next_token.UNION.Node.stor, tokens[(*token_cnt) + 1].stor);
                            
                            (*token_cnt)++;
                        }
                    /*mmstacksz += sizeof(next_token);
                    mmstack.UNode.Nodelst.nodelst = realloc(mmstack.UNode.Nodelst.nodelst, mmstacksz);
                    mmstack.UNode.Nodelst.nodelst[2] = next_token;
                    // IMPORTANT ======== DO NOT FORGET
                    mmstack.UNode.Nodelst.nodeln = 3;
                    // ================================*/
                    
                    mmstacksz += sizeof(next_token);
                    mmstack.UNION.Nodes.nodes = realloc(mmstack.UNION.Nodes.nodes, mmstacksz);
                    mmstack.UNION.Nodes.nodes[mmstack.UNION.Nodes.nodeln - 1] = next_token;
                    mmstack.UNION.Nodes.nodeln++;
                    
                    // update the memory
                    /*mem->UNode.Nodelst.nodelst[mem->UNode.Nodelst.nodeln] = mmstack;*/
                    
                    umem(mem, &mmstack, memsz);
                    
                    // update mstack with mmstack
                    /*mstacksz += sizeof(mmstack);
                    mstack.UNode.Nodelst.nodelst = realloc(mstack.UNode.Nodelst.nodelst, mstacksz);
                    mstack.UNode.Nodelst.nodelst[mstack.UNode.Nodelst.nodeln] = mmstack;
                    mstack.UNode.Nodelst.nodeln++;*/
                    
                    mstacksz += sizeof(mmstack);
                    mstack.UNION.Nodes.nodes = realloc(mmstack.UNION.Nodes.nodes, mmstacksz);
                    mmstack.UNION.Nodes.nodes[mmstack.UNION.Nodes.nodeln - 1] = mmstack;
                    mmstack.UNION.Nodes.nodeln++;
                    
                }
            
            token = tokens[*token_cnt];
            (*token_cnt)++;
        }
}

NodeMaster parse_assignment(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
{
    /*NodeList token = mem->UNode.Nodelst.nodelst[mem->UNode.Nodelst.nodeln];
    Token next_token = tokens[(*token_cnt) + 1];*/
    
    NodeMaster token = mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1];
    Token next_token = tokens[(*token_cnt) + 1];
    
    // mstack --> ministack, like a branch in a tree
    /*NodeList mstack;
    mstack.UNode.Nodelst.nodelst = NULL;
    mstack.UNode.Nodelst.nodeln = 0;
    size_t mstacksz = 0;*/
    
    size_t mstacksz = 0;
    NodeMaster mstack;
    mstack.utype = NODES;
    mstack.UNION.Nodes.nodes = NULL;
    mstack.UNION.Nodes.nodeln = 0;
    
    // assign the first value (the terminal index) to the assignment token [EQUAL]
    /*NodeList assig;
    assig.node.type = next_token.type;
    strcpy(assig.node.stor, next_token.stor);*/
    
    NodeMaster assig;
    assig.utype = NODE;
    assig.UNION.Node.type = next_token.type;
    strcpy(assig.UNION.Node.stor, next_token.stor);
    
    // copy that value into the stack (actually placing it into the stack)
    /*mstacksz += sizeof(assig);
    mstack.UNode.Nodelst.nodelst = realloc(mstack.UNode.Nodelst.nodelst, mstacksz);
    mstack.UNode.Nodelst.nodelst[0] = assig;*/
    
    mstacksz += sizeof(assig);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = assig;
    mstack.UNION.Nodes.nodeln++;
    
    // assign left node as a new object
    /*Token token = tokens[token_cnt];
    NodeList ltoken;
    ltoken.node.type = token.type;
    strcpy(ltoken.node.stor, token.stor);*/
    
    // append ltoken object into the stack
    /*mstacksz += sizeof(token);
    mstack.UNode.Nodelst.nodelst = realloc(mstack.UNode.Nodelst.nodelst, mstacksz);
    mstack.UNode.Nodelst.nodelst[1] = token;*/

    mstacksz += sizeof(token);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = token;
    mstack.UNION.Nodes.nodeln++;
    
    // find the assignment value
    /**token_cnt += 2;
    NodeList value = parse_expr(tokens, token_cnt, mem);*/
    
    *token_cnt += 2;
    NodeMaster value = parse_expr(tokens, token_cnt, mem, memsz);
    
    // append value into the stack
    /*mstacksz += sizeof(value);
    mstack.UNode.Nodelst.nodelst = realloc(mstack.UNode.Nodelst.nodelst, mstacksz);
    mstack.UNode.Nodelst.nodelst[2] = value;
    mstack.UNode.Nodelst.nodeln = 3;*/
    
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
    /*size_t _stacksz = 1;
    NodeList *_stack;// = malloc(sizeof(NodeList) * _stacksz);
    _stack->type = PROGRAM;
    _stack->UNode.Nodelst.nodelst = NULL;
    _stack->UNode.Nodelst.nodeln = 0;*/
    
    size_t stacksz = 0;
    NodeMaster _stack;
    _stack.utype = NODES;
    _stack.UNION.Nodes.type = PROGRAM;
    _stack.UNION.Nodes.nodes = NULL;
    _stack.UNION.Nodes.nodeln = 0;
    
    // initialize the entire memory _stack
    /*size_t memsz = 2;
    NodeList *mem = malloc(sizeof(NodeList) * memsz);
    //mem[0].UNode.Nodelst.nodelst = NULL;
    mem[0].type = PROGRAM;
    mem[0].UNode.Nodelst.nodeln = 1;*/
    
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
            NodeMaster tn;
            tn.utype = NODE;
            tn.UNION.Node.type = TOKEN_TYPE;
            strcpy(tn.UNION.Node.stor, token.stor);
            //    then store it in memory
            umem(&mem, &tn, &memsz);
            
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
                            /*_stacksz += sizeof(mstack);
                            _stack->UNode.Nodelst.nodelst = realloc(_stack->UNode.Nodelst.nodelst, _stacksz);
                            _stack->UNode.Nodelst.nodelst[_stack->UNode.Nodelst.nodeln + 1] = mstack;
                            _stack->UNode.Nodelst.nodeln++;*/
                            
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
                           /* _stacksz += sizeof(mstack);
                            _stack->UNode.Nodelst.nodelst = realloc(_stack->UNode.Nodelst.nodelst, _stacksz);
                            _stack->UNode.Nodelst.nodelst[_stack->UNode.Nodelst.nodeln + 1] = mstack;
                            _stack->UNode.Nodelst.nodeln++;*/
                            
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