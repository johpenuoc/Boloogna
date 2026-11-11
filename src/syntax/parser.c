#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "coms/common.h"


void callfnc(enum _TYPE_ type, void *tokens, int id)
{
    switch (c)
    {
        case LPAREN:
            break;
        case RPAREN:
            break;
        case LBRACE:
            break;
        case RBRACE:
            break;
        case COMMA:
            break;
        case DECIMAL:
            break;
        case COLON:
            break;
        case SCOLON:
            break;
        case ADD:
            break;
        case SUB:
            break;
        case STAR:
            break;
        case DIV:
             
            break;
        case EQUAL:
            break;
        case POWER:
            break;
        case GTHAN:
            break;
        case LTHAN:
            break;
            
        case EQUIVALENT:
            break;
        case GTHAN_OR_EQUAL_TO:
            break;
        case LTHAN_OR_EQUAL_TO:
            break;
    }

}


List ast(List *tokens)
{
    Branch *branch;
    
    int i = 0;
    while (tokens->length - 1 > i)
        {
            Token *token = (Token *)tokens->element;
            const char *TOKEN_STOR = token[i].stor;
            const enum _TYPE_ TOKEN_TYPE = token[i].type;
            
            // there are 29 terminals
            if (TOKEN_TYPE < 29)
                {
                    
                }
            else
                {
                    
                }
            
            i++;
        }
    
    // temporary
    free(branch->children);
}