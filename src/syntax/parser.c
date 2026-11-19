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

NodeMaster parse_list(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);
NodeMaster parse_until(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);
NodeMaster parse_IDlinker(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);
NodeMaster parse_scope(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz);

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
    mstack.UNION.Nodes.type = EXPRESSION;
    
    while (true)
        {
            if (token.type == RPAREN || token.type == SCOLON || token.type == COMMA || token.type == LBRACE || token.type == RBRACKET)// || token.type == AND || token.type == OR) 
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
            else if (token.type == IDENTIFIER && tokens[(*token_cnt) + 1].type == LPAREN)
                {
                    // in other words, this checks whether its a function or not
                    NodeMaster func = parse_func(tokens, token_cnt, mem, memsz);
                    umem(mem, func, memsz);
                    
                    (*token_cnt)--;
                }
            else if (token.type == LBRACKET)
                {
                    // basically, we're in a list
                    (*token_cnt)++;
                    NodeMaster list = parse_list(tokens, token_cnt, mem, memsz);
                    
                    umem(mem, list, memsz);
                    
                    (*token_cnt)--;
                }
            else if (token.type == NOT)
                {
                    // this is a mini-mini stack --> [OP][x][y]                
                    size_t mmstacksz = 0;
                    NodeMaster mmstack;
                    mmstack.utype = NODES;
                    mmstack.UNION.Nodes.nodes = NULL;
                    mmstack.UNION.Nodes.nodeln = 0;
                    mmstack.UNION.Nodes.type = _NOT;
                    
                    // THE NEXT TOKEN MUST BE LPAREN
                    // [NOT] is syntactically like a 
                    // function. It must be used as:
                    // if !(condition) {} 
                    // it will only invert what is 
                    // within [LPAREN] [expr] [RPAREN]
                    
                    (*token_cnt) += 2;
                    
                    NodeMaster _tmp = parse_expr(tokens, token_cnt, mem, memsz);
                    mmstacksz += sizeof(_tmp);
                    mmstack.UNION.Nodes.nodes = realloc(mmstack.UNION.Nodes.nodes, mmstacksz);
                    mmstack.UNION.Nodes.nodes[mmstack.UNION.Nodes.nodeln] = _tmp;
                    mmstack.UNION.Nodes.nodeln++;
                    
                    umem(mem, mmstack, memsz);
                    
                    (*token_cnt)--;
                    
                }
            else if (token.type == ADD || token.type == SUB || token.type == STAR || token.type == DIV || token.type == AND || token.type == OR || token.type == EQUIVALENT || token.type == GTHAN_OR_EQUAL_TO || token.type == LTHAN_OR_EQUAL_TO || token.type == GTHAN || token.type == LTHAN)
                {
                    // this is a mini-mini stack --> [OP][x][y]                
                    size_t mmstacksz = 0;
                    NodeMaster mmstack;
                    mmstack.utype = NODES;
                    mmstack.UNION.Nodes.nodes = NULL;
                    mmstack.UNION.Nodes.nodeln = 0;
                    mmstack.UNION.Nodes.type = ASSIGNMENT;
                    
                    // fetch the previous token from memory
                    NodeMaster prev_token = mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1];
                    
                    // assign the first value (the terminal index) to the assignment token [ ADD || SUB || STAR || DIV || AND || OR ]      
                    NodeMaster term;
                    term.utype = NODE;
                    term.UNION.Node.type = token.type;
                    _strcpy(&term, token.stor);
                    printf("token: %s\n", tokens[(*token_cnt)].stor);
                    
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
                    else if (tokens[(*token_cnt) + 1].type == IDENTIFIER && tokens[(*token_cnt) + 2].type == LPAREN)
                        {
                            (*token_cnt)++;
                            // in other words, this checks whether its a function or not
                            NodeMaster func = parse_func(tokens, token_cnt, mem, memsz);
                            umem(mem, func, memsz);
                            
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
                }
            else amem(mem, memsz, tokens, token_cnt);
                
            (*token_cnt)++;
            token = tokens[*token_cnt];
        }
}

NodeMaster _parse_condition(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
{
    size_t condsz = 0;
    NodeMaster cond;
    cond.utype = NODES;
    cond.UNION.Nodes.nodes = NULL;
    cond.UNION.Nodes.nodeln = 0;
    cond.UNION.Nodes.type = CONDITION;
    
    while (true)
        {   
            NodeMaster expr = parse_expr(tokens, token_cnt, mem, memsz);
            
            condsz += sizeof(expr);
            cond.UNION.Nodes.nodes = realloc(cond.UNION.Nodes.nodes, condsz);
            cond.UNION.Nodes.nodes[cond.UNION.Nodes.nodeln] = expr;
            cond.UNION.Nodes.nodeln++;
            
            (*token_cnt)--;
            
            const enum _TYPE_ TT = tokens[*token_cnt].type;
            if (TT == SCOLON || TT == LBRACE)
                {
                    return cond;
                }
        }
}

NodeMaster parse_assignment(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
{       
    // =================================================
    // THIS FUNCTION EXPECTS TO BE ON [IDENTIFIER] FIRST
    // =================================================
    
    NodeMaster token = mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1];
    Token next_token = tokens[(*token_cnt) + 1];
    
    // mstack --> ministack, like a branch in a tree   
    size_t mstacksz = 0;
    NodeMaster mstack;
    mstack.utype = NODES;
    mstack.UNION.Nodes.nodes = NULL;
    mstack.UNION.Nodes.nodeln = 0;
    mstack.UNION.Nodes.type = ASSIGNMENT;
    
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
    printf("assig: %s\n", tokens[(*token_cnt)].stor);

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
    mstack.UNION.Nodes.type = FUNCTION_CALL;
      
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
    args.UNION.Nodes.type = ARGUMENTS;
    
    *token_cnt += 3;
    bool update = false;
    while (true)
        {       
            token = tokens[*token_cnt];
            
            if (tokens[(*token_cnt) - 1].type == RPAREN) break;
    
            if (!update)
                {
                    amem(mem, memsz, tokens, token_cnt);
                    update = true;
                    (*token_cnt)--;
                }
            NodeMaster arg = parse_expr(tokens, token_cnt, mem, memsz);
            umem(mem, arg, memsz);
            
            argsz += sizeof(arg);
            args.UNION.Nodes.nodes = realloc(args.UNION.Nodes.nodes, argsz);
            args.UNION.Nodes.nodes[args.UNION.Nodes.nodeln] = arg;
            args.UNION.Nodes.nodeln++;
        }
    
    /*NodeMaster tmp = parse_expr(tokens, token_cnt, mem, memsz);
    umem(mem, tmp, memsz);*/
    umem(mem, args, memsz);
    
    // append it into mstack
    sto_mstack(&mstack, &mstacksz, args);
    
    return mstack;
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
    mstack.UNION.Nodes.type = ID_LINKER;
    
    while (true)
        { 
            if (token.type == SCOLON || token.type != IDENTIFIER && token.type != DECIMAL) 
                {                   
                    NodeMaster mmstack = mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1];
                    mstacksz += sizeof(mmstack);
                    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                    mstack.UNION.Nodes.nodeln++;
                    
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
                    mmstack.UNION.Nodes.type = ID_LINKER;
                    
                    // fetch the previous token from memory
                    NodeMaster prev_token = mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1];
                    
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
                    if (tokens[(*token_cnt) + 2].type == LPAREN)
                        {
                            (*token_cnt)++;
                            next_token = parse_func(tokens, token_cnt, mem, memsz);
                            
                            (*token_cnt)--;
                        }
                    else
                        {
                            next_token.utype = NODE;
                            next_token.UNION.Node.type = tokens[(*token_cnt) + 1].type;
                            _strcpy(&next_token, tokens[(*token_cnt) + 1].stor);
                            
                            (*token_cnt)++;
                        };
                    
                    mmstacksz += sizeof(next_token);
                    mmstack.UNION.Nodes.nodes = realloc(mmstack.UNION.Nodes.nodes, mmstacksz);
                    mmstack.UNION.Nodes.nodes[mmstack.UNION.Nodes.nodeln] = next_token;
                    mmstack.UNION.Nodes.nodeln++;
                    
                    // update the memory
                    umem(mem, mmstack, memsz);
                }
            else amem(mem, memsz, tokens, token_cnt);
                
            (*token_cnt)++;
            token = tokens[*token_cnt];
        }
    
}

NodeMaster parse_if(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
{
    // =================================================
    // THIS FUNCTION EXPECTS TO BE ON [IF] FIRST
    // =================================================
    
    // initialize mstack
    size_t mstacksz = 0;
    NodeMaster mstack;
    mstack.utype = NODES;
    mstack.UNION.Nodes.nodes = NULL;
    mstack.UNION.Nodes.nodeln = 0;
    mstack.UNION.Nodes.type = _IF;
    
    // grab first token
    // which should be the [IF] token, btw
    NodeMaster if_token;
    if_token.utype = NODE;
    if_token.UNION.Node.type = tokens[*token_cnt].type;
    _strcpy(&if_token, tokens[*token_cnt].stor);
    
    // append if_token into mstack 
    mstacksz += sizeof(if_token);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = if_token;
    mstack.UNION.Nodes.nodeln++;
    
    // parse the conditional statement
    (*token_cnt)++;
    size_t condssz = 0;
    NodeMaster conds;
    conds.utype = NODES;
    conds.UNION.Nodes.type = CONDITION;
    conds.UNION.Nodes.nodes = NULL;
    conds.UNION.Nodes.nodeln = 0;
    
    NodeMaster cond = _parse_condition(tokens, token_cnt, mem, memsz);
    
    // append cond into mstack
    mstacksz += sizeof(cond);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = cond;
    mstack.UNION.Nodes.nodeln++;
    
    // parse scope
    (*token_cnt)++;
    NodeMaster scope = parse_scope(tokens, token_cnt, mem, memsz);
    
    // append scope into mstack
    mstacksz += sizeof(scope);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = scope;
    mstack.UNION.Nodes.nodeln++;
    
    // =============== ELSE ===============
    // this is an optional token.
    // we will check for an else, then a possible IF afterwards.
    // if IF exists, call a parse_if at that token.
    // if a scope exists after ELSE, call parse_scope
    
    // checking if an ELSE exists at all
    (*token_cnt)++;
    if (!(tokens[*token_cnt].type == ELSE)) 
        {
          (*token_cnt)--;
          return mstack;  
        }
    
    size_t else_tokensz = 0;
    NodeMaster else_token;
    else_token.utype = NODES;
    else_token.UNION.Nodes.type = _ELSE;
    else_token.UNION.Nodes.nodes = NULL;
    else_token.UNION.Nodes.nodeln = 0;

    // check whether there is an IF after the ELSE
    (*token_cnt)++;
    if (tokens[*token_cnt].type == IF)
        {
            NodeMaster _if_token = parse_if(tokens, token_cnt, mem, memsz);
            
            else_tokensz += sizeof(_if_token);
            else_token.UNION.Nodes.nodes = realloc(else_token.UNION.Nodes.nodes, else_tokensz);
            else_token.UNION.Nodes.nodes[else_token.UNION.Nodes.nodeln] = _if_token;
            else_token.UNION.Nodes.nodeln++;
        }
    else
        {
            // parse scope
            (*token_cnt)++;
            NodeMaster _scope = parse_scope(tokens, token_cnt, mem, memsz);
            
            // append scope into mstack
            else_tokensz += sizeof(_scope);
            else_token.UNION.Nodes.nodes = realloc(else_token.UNION.Nodes.nodes, else_tokensz);
            else_token.UNION.Nodes.nodes[else_token.UNION.Nodes.nodeln] = _scope;
            else_token.UNION.Nodes.nodeln++;
        }
    
    mstacksz += sizeof(else_token);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = else_token;
    mstack.UNION.Nodes.nodeln++;
    
    return mstack;
}

// anything that's contained in '{}'
NodeMaster parse_scope(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
{
    // =================================================
    // THIS FUNCTION EXPECTS TO BE ON THE TOKEN AFTER
    // [LBRACE] (A.K.A., THE FIRST TOKEN IN A SCOPE)
    // =================================================
    
    // mstack --> ministack, like a branch in a tree 
    size_t mstacksz = 0;
    NodeMaster mstack;
    mstack.utype = NODES;
    mstack.UNION.Nodes.nodes = NULL;
    mstack.UNION.Nodes.nodeln = 0;
    mstack.UNION.Nodes.type = SCOPE;
    
    Token token = tokens[*token_cnt];
    Token next_token = tokens[(*token_cnt) + 1];
    
    while (true)
        {   
            // checking if any are terminal
            const enum _TYPE_ TOKEN_TYPE = token.type;
            const enum _TYPE_ NEXT_TOKEN_TYPE = next_token.type;
            
            amem(mem, memsz, tokens, token_cnt);
            
            switch (TOKEN_TYPE)
            {
                case RBRACE:
                    // if this is un-commented, it will always have TYPE: [RPAREN]
                    // at the end of the mstack.
                    
                    /*NodeMaster mmstack = mem->UNION.Nodes.nodes[mem->UNION.Nodes.nodeln - 1];
                    mstacksz += sizeof(mmstack);
                    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                    mstack.UNION.Nodes.nodeln++;*/
                    
                    return mstack;
                case UNTIL:
                    {
                        NodeMaster mmstack = parse_until(tokens, token_cnt, mem, memsz);
                        
                        // append it to the stack       
                        mstacksz += sizeof(mmstack);
                        mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                        mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                        mstack.UNION.Nodes.nodeln++; 
                    }
                    break;
                case IDENTIFIER:  
                    if (NEXT_TOKEN_TYPE == LPAREN)
                        {
                            NodeMaster mmstack = parse_func(tokens, token_cnt, mem, memsz);
                            
                            // append it to the stack       
                            mstacksz += sizeof(mmstack);
                            mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                            mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                            mstack.UNION.Nodes.nodeln++;
                            
                        }
                    else if (NEXT_TOKEN_TYPE == EQUAL || NEXT_TOKEN_TYPE == ADD_EQ || NEXT_TOKEN_TYPE == SUB_EQ || NEXT_TOKEN_TYPE == STAR_EQ || NEXT_TOKEN_TYPE == DIV_EQ)
                        {
                            NodeMaster mmstack = parse_assignment(tokens, token_cnt, mem, memsz);
                            
                            // append it to the stack       
                            mstacksz += sizeof(mmstack);
                            mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                            mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                            mstack.UNION.Nodes.nodeln++;
                            
                            (*token_cnt)--;

                            break;
                        }
                    else if (NEXT_TOKEN_TYPE == DECIMAL)
                        {
                            NodeMaster mmstack = parse_IDlinker(tokens, token_cnt, mem, memsz);
                        
                            // append it to the stack       
                            mstacksz += sizeof(mmstack);
                            mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                            mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                            mstack.UNION.Nodes.nodeln++;
                        }
                    else if (NEXT_TOKEN_TYPE == ADD || NEXT_TOKEN_TYPE == SUB || NEXT_TOKEN_TYPE == STAR || NEXT_TOKEN_TYPE == DIV)
                        {
                            NodeMaster mmstack = parse_expr(tokens, token_cnt, mem, memsz);
                            
                            // append it to the stack       
                            mstacksz += sizeof(mmstack);
                            mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                            mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                            mstack.UNION.Nodes.nodeln++;
                        }
                    break;
                case LPAREN:
                    // parse_expr
                    break;
                case LBRACE:
                    {
                        (*token_cnt)++;
                        NodeMaster mmstack = parse_scope(tokens, token_cnt, mem, memsz);
                        
                        // append it to the stack       
                        mstacksz += sizeof(mmstack);
                        mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                        mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                        mstack.UNION.Nodes.nodeln++;
                    }
                    break;
                case IF:
                    {
                        NodeMaster mmstack = parse_if(tokens, token_cnt, mem, memsz);
                        
                        // append it to the stack       
                        mstacksz += sizeof(mmstack);
                        mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
                        mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
                        mstack.UNION.Nodes.nodeln++;
                        
                        //(*token_cnt)--;
                    }
                    break;
                case EQUAL:
                    // prase_expr
                    break;
            }
        
        //amem(mem, memsz, tokens, token_cnt);
        (*token_cnt)++;
        
        token = tokens[*token_cnt];
        next_token = tokens[(*token_cnt) + 1];
        }
        
    return mstack;
}

NodeMaster parse_dfn(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
{
    // =================================================
    // THIS FUNCTION EXPECTS TO BE ON [DEF_FN]
    // =================================================
    
    // mstack --> ministack, like a branch in a tree 
    size_t mstacksz = 0;
    NodeMaster mstack;
    mstack.utype = NODES;
    mstack.UNION.Nodes.nodes = NULL;
    mstack.UNION.Nodes.nodeln = 0;
    mstack.UNION.Nodes.type = _DEF_FN;
    
    (*token_cnt)++;
    NodeMaster declar = parse_func(tokens, token_cnt, mem, memsz);
    (*token_cnt)++;
    
    mstacksz += sizeof(declar);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = declar;
    mstack.UNION.Nodes.nodeln++;
    
    // Now we get the main body of the function
    NodeMaster body = parse_scope(tokens, token_cnt, mem, memsz);
    
    mstacksz += sizeof(body);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = body;
    mstack.UNION.Nodes.nodeln++;
    
    return mstack;
}

NodeMaster parse_until(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
{
    // =================================================
    // THIS FUNCTION EXPECTS TO BE ON [DEF_FN]
    // =================================================
    
    // mstack --> ministack, like a branch in a tree 
    size_t mstacksz = 0;
    NodeMaster mstack;
    mstack.utype = NODES;
    mstack.UNION.Nodes.nodes = NULL;
    mstack.UNION.Nodes.nodeln = 0;
    mstack.UNION.Nodes.type = LOOP;
    
    (*token_cnt)++;
    NodeMaster cond = _parse_condition(tokens, token_cnt, mem, memsz);
    (*token_cnt)++;
    
    mstacksz += sizeof(cond);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = cond;
    mstack.UNION.Nodes.nodeln++;
    
    // Now we get the main body of the function
    NodeMaster body = parse_scope(tokens, token_cnt, mem, memsz);
    
    mstacksz += sizeof(body);
    mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
    mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = body;
    mstack.UNION.Nodes.nodeln++;
    
    return mstack;
}

NodeMaster parse_list(Token *tokens, int *token_cnt, NodeMaster *mem, size_t *memsz)
{
    // =================================================
    // THIS FUNCTION EXPECTS TO BE ON [LBRACKET]
    // =================================================
    
    // mstack --> ministack, like a branch in a tree 
    size_t mstacksz = 0;
    NodeMaster mstack;
    mstack.utype = NODES;
    mstack.UNION.Nodes.nodes = NULL;
    mstack.UNION.Nodes.nodeln = 0;
    mstack.UNION.Nodes.type = SCOPE;
    
    Token token = tokens[*token_cnt];
    
    while (true)
        {              
            printf("before %s\n", tokens[*token_cnt].stor);
            NodeMaster mmstack = parse_expr(tokens, token_cnt, mem, memsz);
            printf("after %s\n", tokens[*token_cnt].stor);
                            
            // append it to the stack       
            mstacksz += sizeof(mmstack);
            mstack.UNION.Nodes.nodes = realloc(mstack.UNION.Nodes.nodes, mstacksz);
            mstack.UNION.Nodes.nodes[mstack.UNION.Nodes.nodeln] = mmstack;
            mstack.UNION.Nodes.nodeln++;
            
            if (tokens[(*token_cnt) - 1].type == RBRACKET) return mstack;
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
    
    while (token_cnt < tokens->length - 2)
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
                            // PROBLEM HERE    
                            stacksz += sizeof(mstack);
                            _stack.UNION.Nodes.nodes = realloc(_stack.UNION.Nodes.nodes, stacksz);
                            _stack.UNION.Nodes.nodes[_stack.UNION.Nodes.nodeln] = mstack;
                            _stack.UNION.Nodes.nodeln++;     
                        }
                    else if (NEXT_TOKEN_TYPE == DECIMAL)
                        {
                            NodeMaster mstack = parse_IDlinker(_tokens, ptoken_cnt, &mem, &memsz);
                        
                            // append it to the stack       
                            stacksz += sizeof(mstack);
                            _stack.UNION.Nodes.nodes = realloc(_stack.UNION.Nodes.nodes, stacksz);
                            _stack.UNION.Nodes.nodes[_stack.UNION.Nodes.nodeln] = mstack;
                            _stack.UNION.Nodes.nodeln++;
                        }
                    else if (NEXT_TOKEN_TYPE == ADD || NEXT_TOKEN_TYPE == SUB || NEXT_TOKEN_TYPE == STAR || NEXT_TOKEN_TYPE == DIV)
                        {                        
                            NodeMaster mstack = parse_expr(_tokens, ptoken_cnt, &mem, &memsz);
                            
                            // append it to the stack       
                            stacksz += sizeof(mstack);
                            _stack.UNION.Nodes.nodes = realloc(_stack.UNION.Nodes.nodes, stacksz);
                            //                                                - 1
                            _stack.UNION.Nodes.nodes[_stack.UNION.Nodes.nodeln] = mstack;
                            _stack.UNION.Nodes.nodeln++;
                        }
                    break;
                case UNTIL:
                    {
                        NodeMaster mstack = parse_until(_tokens, ptoken_cnt, &mem, &memsz);
                        
                        // append it to the stack       
                        stacksz += sizeof(mstack);
                        _stack.UNION.Nodes.nodes = realloc(_stack.UNION.Nodes.nodes, stacksz);
                        _stack.UNION.Nodes.nodes[_stack.UNION.Nodes.nodeln] = mstack;
                        _stack.UNION.Nodes.nodeln++;
                        
                        token_cnt++;
                    }
                    break;
                case IF:
                    NodeMaster __mstack = parse_if(_tokens, ptoken_cnt, &mem, &memsz);
                    
                    // append it to the stack       
                    stacksz += sizeof(__mstack);
                    _stack.UNION.Nodes.nodes = realloc(_stack.UNION.Nodes.nodes, stacksz);
                    _stack.UNION.Nodes.nodes[_stack.UNION.Nodes.nodeln] = __mstack;
                    _stack.UNION.Nodes.nodeln++;
                    
                    token_cnt++;
                    break;
                case DEF_FN:
                    {
                        NodeMaster mstack = parse_dfn(_tokens, ptoken_cnt, &mem, &memsz);
                        
                        stacksz += sizeof(mstack);
                        _stack.UNION.Nodes.nodes = realloc(_stack.UNION.Nodes.nodes, stacksz);
                        //                                                - 1
                        _stack.UNION.Nodes.nodes[_stack.UNION.Nodes.nodeln] = mstack;
                        _stack.UNION.Nodes.nodeln++;
                    }
                    break;
                case LPAREN:
                    // parse_expr
                    break;
                case LBRACE:
                    NodeMaster _mstack = parse_scope(_tokens, ptoken_cnt, &mem, &memsz);
                    
                    // append it to the stack       
                    stacksz += sizeof(_mstack);
                    _stack.UNION.Nodes.nodes = realloc(_stack.UNION.Nodes.nodes, stacksz);
                    _stack.UNION.Nodes.nodes[_stack.UNION.Nodes.nodeln] = _mstack;
                    _stack.UNION.Nodes.nodeln++;

                    token_cnt++;
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