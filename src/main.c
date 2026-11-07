#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "coms/common.h"

const char *ALPHA = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const char *TERM = "(){},.:;+-*/=^><";
const char *MODE = "\"#";

bool is_in(char *string, char c, int string_length)
{
    for (int i = 0; i < string_length; ++i)
        {
            if (string[i] == c)
                {
                    return true;
                }
        }
    
    return false;
}

bool only_numbers(char *string, int string_length)
{
    for (int i = 0; i < string_length; ++i)
        {
            if (is_in(ALPHA, string[i], 43))
                {
                    return false;
                }
        }
    
    return true;
}

enum _TYPE_ {
    LPAREN, RPAREN, LBRACE, RBRACE,
    COMMA, DECIMAL, COLON, SCOLON, 
    ADD, SUB, STAR, DIV, EQUAL, 
    POWER, GTHAN, LTHAN,
    
    EQUIVALENT, GTHAN_OR_EQUAL_TO,
    LTHAN_OR_EQUAL_TO, 
    
    STRING, IDENTIFIER, NUMBER,
    
    AND, OR, NOT, GOTO, UNTIL, IF, ELSE
};

typedef struct Token
{
    enum _TYPE_ type;
    int line;
    
} Token;

List preprocessor(FILE *file)
{
    int char_max = 2;
    char *buf = malloc(sizeof(char) * char_max);
    int cnt = 0;
    
    bool in_mode = false;
    char mode_type;
    
    char c;
    while ((c = fgetc(file)) != EOF)
        {
            if (cnt >= char_max - 1)
                {
                    char_max += 2;
                    char *checker = realloc(buf, char_max);
                    
                    if (checker == NULL)
                        {
                            break;
                        }
                    buf = checker;
                }
                
            if (c == '\n') continue;
            if (!in_mode)
                {   
                    if (is_in(MODE, c, 2))
                        {
                            in_mode = true;
                            mode_type = c;
                            
                            buf[cnt] = ' ';
                            cnt++;
                        }
                    
                    bool is_term = is_in(TERM, c, 16);
                    if (!in_mode && is_term)
                        {
                            buf[cnt] = ' ';
                            cnt++;
                        }
                        
                    buf[cnt] = c;
                    cnt++;
                    
                    if (is_term)
                        {
                            buf[cnt] = ' ';
                            cnt++;
                        }
                }
            else
                {              
                    buf[cnt] = c;
                    cnt++;   
                    
                    if (c == mode_type && !(buf[cnt - 2] == '\\'))
                        {
                            in_mode = false;
                            buf[cnt] = ' ';
                            cnt++;
                        }
                }
        }
        
    return (List){ .element = buf, .length = cnt };
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
        
    
    List processed = preprocessor(file);
        
    for (int i = 0; i < processed.length; ++i)
        {
            printf("%c", ((char *)processed.element)[i]);
        }
        
    free(processed.element);
    
    /*for (int i = 0; i < token_cnt; ++i)
        {
            printf("Token ID[%d]: %d\n", i, token[i].type);
        }*/
    
    return 0;
}