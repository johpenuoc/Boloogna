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

NodeMaster parse_IDlinker(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);

// save to mstack
void sto_mstack(NodeMaster *mstack, size_t *mstacksz, NodeMaster node_s)
{
    *mstacksz += sizeof(node_s);
    mstack->UNION.Nodes.nodes = realloc(mstack->UNION.Nodes.nodes, *mstacksz);
    mstack->UNION.Nodes.nodes[mstack->UNION.Nodes.nodeln] = node_s;
    mstack->UNION.Nodes.nodeln++;
}

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
    //printf("added type %s with stor %s into memory at index %d\n", _utypes[x.utype], x.UNION.Node.stor, mem->UNION.Nodes.nodeln);
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
            printf("-> %s\n", token.stor);
            if (token.type == RPAREN || token.type == SCOLON || token.type == COMMA) 
                {                   
                    NodeMaster mmstack = mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1];
                    mstacksz += sizeof(mmstack);
                    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                    mstack.UNION.Nodes.nodeln++;
                    
                    (*token_cnt)++;
                    return mstack; 
                }
            else if (token.type == LPAREN)
                {
                    (*token_cnt)++;
                    
                    NodeMaster tmp = parse_expr(tokens, token_cnt, mem, memsz);
                    umem(mem, tmp, memsz);
                    
                    (*token_cnt)--;
                }
            else if (token.type == DECIMAL)
                {
                    // parse_IDlinker expects to start on the DECIMAL
                    NodeMaster idlk = parse_IDlinker(tokens, token_cnt, mem, memsz);
                    umem(mem, idlk, memsz);
                    
                    // parse_expr (the function we're in) expects to go through every token.
                    // parse_IDlinker puts us one ahead. hence '--'
                    (*token_cnt)--;
                }
                         // might have to be tokens[(*token_cnt) + 1].type == IDENTIFIER
            else if (token.type == LPAREN && tokens[*token_cnt].type == IDENTIFIER)
                {
                    // in other words, this checks whether its a function or not
                    NodeMaster func = parse_func(tokens, token_cnt, mem, memsz);
                    umem(mem, func, memsz);
                    
                    // parse_expr (the function we're in) expects to go through every token.
                    // parse_IDlinker puts us one ahead. hence '--'
                    (*token_cnt)--;
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
                            (*token_cnt)--;
                        }
                    else if (tokens[(*token_cnt) + 1].type == IDENTIFIER && tokens[(*token_cnt) + 2].type == DECIMAL)
                        {
                            (*token_cnt)++;
                            amem(mem, memsz, tokens, token_cnt);
                            
                            (*token_cnt)++;
                            next_token = parse_IDlinker(tokens, token_cnt, mem, memsz);
                            (*token_cnt)--;
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
                    /*mstacksz += sizeof(mmstack);
                    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                    mstack.UNION.Nodes.nodeln++;*/
                    
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

NodeMaster parse_func(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
{
    // this is the function-name token [IDENTIFIER]
    Token token = tokens[*token_cnt];
    
    // mstack --> ministack, like a branch in a tree 
    size_t mstacksz = 0;
    NodeMaster mstack;
    mstack.utype = NODES;
    mstack.UNION.Nodes.nodes = NULL;
    mstack.UNION.Nodes.nodeln = 0;
      
    // get the function-name identifier
    NodeMaster func_name;
    func_name.utype = NODE;
    func_name.UNION.Node.type = token.type;
    _strcpy(&func_name, token.stor);
    
    // append it into mstack
    sto_mstack(&mstack, &mstacksz, func_name);
    
    // this is to catch each argument in a function call
    size_t argsz = 0;
    NodeMaster args;
    args.utype = NODES;
    args.UNION.Nodes.nodes = NULL;
    args.UNION.Nodes.nodeln = 0;
    
    *token_cnt += 2;
    while (true)
        {       
            token = tokens[*token_cnt];
            
            if (tokens[(*token_cnt) - 1].type == RPAREN) break;
            
            printf(">>> %s\n", token.stor);
    
            NodeMaster arg = parse_expr(tokens, token_cnt, mem, memsz);
            umem(mem, arg, memsz);
            
            argsz += sizeof(arg);
            args.UNION.Nodes.nodes = realloc(args.UNION.Nodes.nodes, argsz);
            args.UNION.Nodes.nodes[args.UNION.Nodes.nodeln] = arg;
            args.UNION.Nodes.nodeln++;
        }
    debugp("here?");
    
    /*NodeMaster tmp = parse_expr(tokens, token_cnt, mem, memsz);
    umem(mem, tmp, memsz);*/
    umem(mem, args, memsz);
    
    // append it into mstack
    sto_mstack(&mstack, &mstacksz, args);
    
    return mstack;
    
    //(*token_cnt)--;
}

//player.box.x
// [.] [player] [[.] [box] [[.] [x]]]
NodeMaster parse_IDlinker(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
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
            /*bool endID;
            enum _TYPE_ tokentype = tokens[*token_cnt].type;
            enum _TYPE_ ntokentype = tokens[(*token_cnt) + 1].type;
            if (*token_cnt > 0 && tokentype == IDENTIFIER && ntokentype == DECIMAL) endID = false;
            else if (*token_cnt > 0 && tokentype == IDENTIFIER && ntokentype != DECIMAL) endID = true;
            else endID = false;*/
            
            if (token.type == SCOLON || token.type != IDENTIFIER && token.type != DECIMAL) 
                {                   
                    NodeMaster mmstack = mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1];
                    mstacksz += sizeof(mmstack);
                    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                    mstack.UNION.Nodes.nodeln++;
                    
                    //(*token_cnt)++;
                    return mstack; 
                }
            else if (token.type == DECIMAL)
                {
                    // this is a mini-mini stack --> [OP][x][y]                
                    size_t mmstacksz = 0;
                    NodeMaster mmstack;
                    mmstack.utype = NODES;
                    mmstack.UNION.Nodes.nodes = NULL;
                    mmstack.UNION.Nodes.nodeln = 0;
                    
                    // fetch the previous token from memory
                    NodeMaster prev_token = mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1];
                    //printf("prev token: %s\n", prev_token.UNION.Node.stor);
                    
                    // assign the first value (the terminal index) to the assignment token [ DECIMAL ]      
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
                    next_token.utype = NODE;
                    next_token.UNION.Node.type = tokens[(*token_cnt) + 1].type;
                    _strcpy(&next_token, tokens[(*token_cnt) + 1].stor);
                    
                    (*token_cnt)++;
                    
                    mmstacksz += sizeof(next_token);
                    mmstack.UNION.Nodes.nodes = realloc(mmstack.UNION.Nodes.nodes, mmstacksz);
                    mmstack.UNION.Nodes.nodes[mmstack.UNION.Nodes.nodeln] = next_token;
                    mmstack.UNION.Nodes.nodeln++;
                    
                    // update the memory
                    umem(mem, mmstack, memsz);
                    
                    // update mstack with mmstack       
                    /*mstacksz += sizeof(mmstack);
                    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                    mstack.UNION.Nodes.nodeln++;*/
                }
            else
                {
                    amem(mem, memsz, tokens, token_cnt);
                }
                
            (*token_cnt)++;
            token = tokens[*token_cnt];
        }
    
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
                            NodeMaster func = parse_func(_tokens, ptoken_cnt, &mem, &memsz);
                            
                            // append it to the stack       
                            stacksz += sizeof(func);
                            _stack.UNION.Nodes.nodes = realloc(_stack.UNION.Nodes.nodes, stacksz);
                            _stack.UNION.Nodes.nodes[ _stack.UNION.Nodes.nodeln] = func;
                            _stack.UNION.Nodes.nodeln++;
                            
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
                            NodeMaster mstack = parse_IDlinker(_tokens, ptoken_cnt, &mem, &memsz);
                        
                            // append it to the stack       
                            stacksz += sizeof(mstack);
                            _stack.UNION.Nodes.nodes = realloc(_stack.UNION.Nodes.nodes, stacksz);
                            _stack.UNION.Nodes.nodes[ _stack.UNION.Nodes.nodeln] = mstack;
                            _stack.UNION.Nodes.nodeln++;
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
                    
                default:
                    token_cnt++;
                    break;
            }
        }
        
    return _stack;
}