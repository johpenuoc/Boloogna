#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

extern char *ALPHA;
extern char *TERM;
extern char *MODE;

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
    
    IDENTIFIER, COMMENT, PROGRAM, MEMORY
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
            enum _TYPE_ type;
            struct TAG *nodes;
            int nodeln;
        } Nodes;
        
    } UNION;
    
} NodeMaster;

void debugp(char *s);
bool is_in(char *string, char c, int string_length);
bool only_numbers(char *string, int string_length);

#endif