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
    
    AND, OR, NOT, GOTO, UNTIL, IF, ELSE,
    
    NUMBER, STRING,
    
    EQUIVALENT, GTHAN_OR_EQUAL_TO,
    LTHAN_OR_EQUAL_TO, NOT_EQUAL_TO,
    
    IDENTIFIER, COMMENT, PROGRAM
};

enum _DATATYPE_
{
    _INT_, _STRING_, _CHAR_, _BOOL_,
    _FLOAT_
};

enum _NODETYPE_
{
    NODE, TERMNODE
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

// ==============================
// ==============================

typedef struct
{
    enum _TYPE_ type;
    char *stor;
    
} Node;

// a nodelist can either be a single node --> [Node] at index one
// or it can hold multiple nodes --> [[Node][[Node_1][Node_2]][Node2]]
typedef struct TAG
{   
    enum _TYPE_ type;
    union
    {
        Node node;
        struct {
            struct TAG *nodelst;
            int nodeln;
        } Nodelst;
    } UNode;
    
} NodeList;

// ==============================
// ==============================

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


