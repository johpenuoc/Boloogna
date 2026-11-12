#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

extern char *ALPHA;
extern char *TERM;
extern char *MODE;

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

typedef struct
{
    enum _TYPE_ type;
    char *stor;
    
} Node;

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

bool is_in(char *string, char c, int string_length);
bool only_numbers(char *string, int string_length);

#endif