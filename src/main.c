#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "coms/common.h"
#include "src/syntax/scan.h"
#include "src/syntax/parser.h"

const char *type_names[] = {
    "LPAREN", "RPAREN", "LBRACE", "RBRACE",
    "COMMA", "DECIMAL", "COLON", "SCOLON", 
    "ADD", "SUB", "STAR", "DIV", "EQUAL", 
    "POWER", "GTHAN", "LTHAN",
    
    "AND", "OR", "NOT", "GOTO", "UNTIL", "IF", "ELSE",
    
    "NUMBER", "STRING",
    
    "EQUIVALENT", "GTHAN_OR_EQUAL_TO", "LTHAN_OR_EQUAL_TO",
    "NOT_EQUAL_TO",
    
    "IDENTIFIER", "COMMENT"
};

const char *stack_type_names[] = {
    "PROGRAM", "MEMORY", "EXPRESSION", "ASSIGNMENT", 
    "FUNCTION", "ARGUMENTS", "ID_LINKER", "FRACTIONAL",
    "SCOPE"
};

const char *utype_names[] = {
    "NODE", "NODES"
};

void printn(NodeMaster *stack, int *depth, char *s)
{
    for (int j = 0; j < *depth; ++j) printf("\t");
    if (stack->utype == NODE)
        {
            printf("TYPE: [%11s] \t STOR: [%9s]\n", type_names[stack->UNION.Node.type], stack->UNION.Node.stor);
            //printf("[%s]", stack->UNION.Node.stor);
        }
    else
        {
            printf("[%9s]\n", stack_type_names[stack->UNION.Nodes.type]);//, stack->UNION.Nodes.nodeln);
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
    
    NodeMaster _stack = stack(&token_list);
    free(token_list.element);
    
    int depth;
    depth = 0;
    char *s;
    printn(&_stack, &depth, s);
    
    // =======================================
    
    /*for (int i = 0; i < token_list.length - 1; ++i)
        {
            printf("[%s] \t %s\n", type_names[token[i].type], token[i].stor);
        }*/
    
    return 0;
}