#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

char *ALPHA = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_\"!=+-/*^";
char *TERM = "(){},.:;+-*/=^><!?";
char *MODE = "\"#";

enum _UTYPE_
{
    NODE, NODES
};

enum _LOW_PRIORITY_
{
    _ADD, _SUB
};

enum _MED_PRIORITY_
{
    _STAR, _DIV
};

enum _HIGH_PRIORITY_
{
    _LPAREN
};

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
    
    IDENTIFIER, COMMENT
};

// these are _TYPE_s which are not found in code but are found after
// stack generation
enum _STACK_TYPE_
{
    PROGRAM, MEMORY, EXPRESSION, ASSIGNMENT, 
    FUNCTION, ARGUMENTS, ID_LINKER, FRACTIONAL,
    SCOPE, STATEMENT, CONDITION, _IF
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

// ==============================
// ==============================

// a nodemaster can either be a single node --> [Node] at index one
// or it can hold multiple nodes --> [[Node][[Node_1][Node_2]][Node2]]

typedef struct TAG
{
    enum _UTYPE_ utype;
    union
    {
        struct
        {
            enum _TYPE_ type;
            char *stor;
        } Node;
        
        struct
        {
            enum _STACK_TYPE_ type;
            struct TAG *nodes;
            int nodeln;
        } Nodes;
        
    } UNION;
    
} NodeMaster;

// ==============================
// ==============================

void debugp(char *s)
{
    printf("DEBUG [%s]\n", s);
}

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


