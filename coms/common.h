#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

extern char *ALPHA;
extern char *TERM;
extern char *MODE;

extern char *type_names[];
extern char *stack_type_names[];
extern char *utypes[];

enum _UTYPE_
{
    NODE, NODES
};

enum _COMMANDS_
{
    LOAD, 
    
    STOR,
    
    PUSH,
    
    _ADD,
    
    _SUB,
    
    _MUL,
    
    _DIV,
};


enum _TYPE_ 
{
    LPAREN, RPAREN, LBRACE, RBRACE,
    LBRACKET, RBRACKET, COMMA, DECIMAL, 
    COLON, SCOLON, ADD, SUB, STAR, DIV, 
    EQUAL, POWER, GTHAN, LTHAN,
    
    AND, OR, NOT, GOTO, UNTIL, IF, ELSE,
    RETURN,
    
    NUMBER, STRING, DEF_FN, DEF_CLS,
    
    EQUIVALENT, GTHAN_OR_EQUAL_TO,
    LTHAN_OR_EQUAL_TO, NOT_EQUAL_TO,
    INHERIT, METADATA, INIT,
    
    ADD_EQ, SUB_EQ, STAR_EQ, DIV_EQ,
    
    IDENTIFIER, COMMENT,
    
    PROGRAM, MEMORY, EXPRESSION, ASSIGNMENT, 
    FUNCTION_CALL, ARGUMENTS, ID_LINKER, FRACTIONAL,
    SCOPE, STATEMENT, CONDITION, GTOET, LTOET,
    LOOP, LIST, LIST_ACC, OPERATION, CLASS, INHERITS,
    META_SCOPE
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

typedef struct 
{
    enum _COMMAND_ cmdt;
    int *argln;
    char *val;
} CMD;

void debugp(char *s);
bool is_in(char *string, char c, int string_length);
bool only_numbers(char *string, int string_length);

#endif