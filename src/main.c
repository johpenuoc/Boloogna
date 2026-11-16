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
    
    "IDENTIFIER", "COMMENT", "PROGRAM", "MEMORY"
};

const char *utype_names[] = {
    "NODE", "NODES"
};

void printn(NodeMaster *stack, int *depth)
{
    for (int j = 0; j < *depth; ++j) printf("\t");
    if (stack->utype == NODE)
        {
            printf("TYPE: [%13s] \t STOR: [%13s]\n", type_names[stack->UNION.Node.type], stack->UNION.Node.stor);
        }
    else
        {
            printf("%d _NODES_\n", stack->UNION.Nodes.nodeln);
            (*depth)++;
            for (int i = 0; i < stack->UNION.Nodes.nodeln; ++i)
                {
                    NodeMaster node_s = stack->UNION.Nodes.nodes[i];
                    printn(&node_s, depth);
                }
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
    printf("out?\n");
    
    printf(": %d\n", _stack.UNION.Nodes.nodeln);
    
    int depth;
    depth = 0;
    printn(&_stack, &depth);
    
    // =======================================
    
    /*for (int i = 0; i < token_list.length - 1; ++i)
        {
            printf("[%s] \t %s\n", type_names[token[i].type], token[i].stor);
        }*/
    
    return 0;
}