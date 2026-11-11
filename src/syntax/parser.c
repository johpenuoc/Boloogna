#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "coms/common.h"

// ====== GRAMMAR ======

void add(Branch *branch, char *prev_expr, char *next_expr, const enum _TYPE_ curr_type)
{
    branch->parent_type = curr_type;
    
    Elem *child1;
    child1->elem = prev_expr;
    Elem *child2;
    child2->elem = next_expr;
    
    branch->children = malloc(sizeof(Elem) * 2);
    Elem *children = (Elem *)branch->children;
    children[0] = *child1;
    children[1] = *child2;
}

// =====================


List ast(List *tokens)
{
    Branch *branch;
    
    int i = 0;
    while (tokens->length - 1 > i)
        {
            Token *token = (Token *)tokens->element;
            const char *TOKEN_STOR = token[i].stor;
            const enum _TYPE_ TOKEN_TYPE = token[i].type;
            
            Branch branch;
            // placing this here makes it work... for some reason?
            printf("At token [%s]\n", TOKEN_STOR);
            switch (TOKEN_TYPE)
                {
                    case ADD:
                        Token *prev_expr = (Token *)(tokens->element);
                        Token *next_expr = (Token *)(tokens->element);
                        
                        add(&branch, prev_expr[i - 1].stor, next_expr[i + 1].stor, TOKEN_TYPE);
                        printf("ADD: %s ADD %s\n", prev_expr[i - 1].stor, next_expr[i + 1].stor);
                        break;
                    case SUB:
                        break;
                    case STAR:
                        break;
                    case DIV:
                        break;
                }
            
            i++;
        }
    
    free(branch->children);
}