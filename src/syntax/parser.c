#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "coms/common.h"

// ====== GRAMMAR ======


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
            //printf("At token [%s]\n", TOKEN_STOR);
            switch (TOKEN_TYPE)
                {
                    case ADD:
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