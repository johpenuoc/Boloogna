#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "coms/common.h"
#include "src/funcs/scan.h"

const char *type_names[] = {
    "LPAREN", "RPAREN", "LBRACE", "RBRACE",
    "COMMA", "DECIMAL", "COLON", "SCOLON", 
    "ADD", "SUB", "STAR", "DIV", "EQUAL", 
    "POWER", "GTHAN", "LTHAN",
    "EQUIVALENT", "GTHAN_OR_EQUAL_TO", "LTHAN_OR_EQUAL_TO", 
    "STRING", "IDENTIFIER", "NUMBER",
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
        
    
    // process the file's data to make it
    // easier to tokenize
    List processed = preprocessor(file);
    
    // tokenization
    List token_list = tokenize(processed);
    Token *token = (Token *)(token_list.element);
    
    for (int i = 0; i < token_list.length - 1; ++i)
        {
            printf("[%s] \t %s\n", type_names[token[i].type], token[i].stor);
        }
    
    
        
    free(processed.element);
    
    /*for (int i = 0; i < token_cnt; ++i)
        {
            printf("Token ID[%d]: %d\n", i, token[i].type);
        }*/
    
    return 0;
}