#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

char *ALPHA = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_\"!=+-/*^";
char *TERM = "(){},.:;+-*/=^><!?";
char *MODE = "\"#";

enum _TYPE_ 
{
    LPAREN, RPAREN, LBRACE, RBRACE,
    COMMA, DECIMAL, COLON, SCOLON, 
    ADD, SUB, STAR, DIV, EQUAL, 
    POWER, GTHAN, LTHAN,
    
    EQUIVALENT, GTHAN_OR_EQUAL_TO,
    LTHAN_OR_EQUAL_TO, NOT_EQUAL_TO,
    
    STRING, IDENTIFIER, NUMBER, COMMENT,
    
    AND, OR, NOT, GOTO, UNTIL, IF, ELSE
};

enum _DATATYPE_
{
    _INT_, _STRING_, _CHAR_, _BOOL_,
    _FLOAT_
};

typedef struct Token
{
    enum _TYPE_ type;
    char *stor;
    int len;
    int line;
    
} Token;

typedef struct List
{
    void *element;
    int length;
    
} List;

typedef struct Elem
{
    // expects only a char*
    void *elem;
    
} Elem;

typedef struct Branch
{
    enum _TYPE_ parent_type;
    void *children;
    
} Branch;

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
    if (string[string_length - 1] == '.') return false;
    
    for (int i = 0; i < string_length; ++i)
        {
            if (is_in(ALPHA, string[i], strlen(ALPHA)))
                {
                    return false;
                }
            
        }
    
    return true;
}


