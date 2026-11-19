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
    LBRACKET, RBRACKET, COMMA, DECIMAL, 
    COLON, SCOLON, ADD, SUB, STAR, DIV, 
    EQUAL, POWER, GTHAN, LTHAN,
    
    AND, OR, NOT, GOTO, UNTIL, IF, ELSE,
    
    NUMBER, STRING, DEF_FN,
    
    EQUIVALENT, GTHAN_OR_EQUAL_TO,
    LTHAN_OR_EQUAL_TO, NOT_EQUAL_TO,
    
    ADD_EQ, SUB_EQ, STAR_EQ, DIV_EQ,
    
    IDENTIFIER, COMMENT
};

// _GTOET / _LTOET mean GTHAN_OR_EQUAL_TO and LTHAN_OR_EQUAL_TO, respectively
enum _STACK_TYPE_
{
    PROGRAM, MEMORY, EXPRESSION, ASSIGNMENT, 
    FUNCTION_CALL, ARGUMENTS, ID_LINKER, FRACTIONAL,
    SCOPE, STATEMENT, CONDITION, _IF, _ELSE, 
    _AND, _OR, _NOT, _GTOET, _LTOET, _DEF_FN,
    LOOP, ADDEQ, SUBEQ, STAREQ, DIVEQ, LIST,
    LIST_ACC
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
            enum _STACK_TYPE_ type;
            struct TAG *nodes;
            int nodeln;
        } Nodes;
        
    } UNION;
    
} NodeMaster;

enum _TYPE_ _type_to_stack_type(enum _TYPE_ type);
void debugp(char *s);
bool is_in(char *string, char c, int string_length);
bool only_numbers(char *string, int string_length);

#endif