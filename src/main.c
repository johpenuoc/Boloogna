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
    
    "EQUIVALENT", "GTHAN_OR_EQUAL_TO", "LTHAN_OR_EQUAL_TO",
    "NOT_EQUAL_TO",
    
    "STRING", "IDENTIFIER", "NUMBER", "COMMENT",
    
    "AND", "OR", "NOT", "GOTO", "UNTIL", "IF", "ELSE"
};


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
    
    ast(&token_list);
    
    // =======================================
    
    for (int i = 0; i < token_list.length - 1; ++i)
        {
            printf("[%s] \t %s\n", type_names[token[i].type], token[i].stor);
        }
    
    return 0;
}