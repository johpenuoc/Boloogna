#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "coms/common.h"
#include "src/syntax/scan.h"
#include "src/syntax/parser.h"

void printn(NodeMaster *stack, int *depth, char *s)
{
    for (int j = 0; j < *depth; ++j) printf("\t");
    if (stack->utype == NODE)
        {
            printf("\033[34mNODE:\033[0m \033[32m[%s]\033[0m", type_names[stack->UNION.Node.type]);
            int tn_len = 14 - strlen(type_names[stack->UNION.Node.type]);
            for (int h = 0; h < tn_len - 1; ++h) printf("-");
            printf(">");
            
            printf("[");
            printf("\033[34m%s\033[0m", stack->UNION.Node.stor);
            printf("]\n");
            //printf("[%s]", stack->UNION.Node.stor);
        }
    else
        {
            printf("\033[31mSTACK: \033[0m");
            printf("\033[31m[\033[0m");
            printf("%s", type_names[stack->UNION.Nodes.type]);//, stack->UNION.Nodes.nodeln);
            printf("\033[31m]\033[0m\n");
            (*depth)++;
            //printf("[");
            for (int i = 0; i < stack->UNION.Nodes.nodeln; ++i)
                {
                    NodeMaster node_s = stack->UNION.Nodes.nodes[i];
                    printn(&node_s, depth, s);
                    //if (i + 1 < stack->UNION.Nodes.nodeln) printf(" ");
                }
            //printf("]");
            (*depth)--;
        }
}


int main(int argc, char **argv)
{
    if (argc < 2)
        {
            printf("No file to execute.\n");
            return 1;
        }
        
    
    FILE *file = fopen(argv[1], "r");
    
    if (file == NULL)
        {
            printf("Unable to open file '%s'\n", argv[1]);
            return 1;
        }
          
    // ============COMPILER STAGES============
    
    List token_list = tokenize(file);
    Token *token = (Token *)(token_list.element);
    
    for (int i = 0; i < token_list.length - 1; ++i)
        {
            printf("[%s] \t %s\n", type_names[token[i].type], token[i].stor);
        }
    
    NodeMaster _stack = stack(&token_list);
    free(token_list.element);
    
    int depth;
    depth = 0;
    char *s;
    printn(&_stack, &depth, s);
    
    // =======================================
    
    // this just prints out the tokens after 'tokenize(file);, line 91'
    /*for (int i = 0; i < token_list.length - 1; ++i)
        {
            printf("[%s] \t %s\n", type_names[token[i].type], token[i].stor);
        }*/
    
    return 0;
}