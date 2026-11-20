#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

char *ALPHA = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_\"!=+-/*^&|";
char *TERM = "(){}[],.:;+-*/=^><!?";
char *MODE = "\"#";

enum _UTYPE_
{
    NODE, NODES
};

enum _COMMAND_
{
    _ADD, _SUB, _MUL, _DIV,
    
    _PUSH, _CALL, _STO, _ID,
    
    _LITERAL,
    
    SPLIT
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

const char *type_names[] = {
    "LPAREN", "RPAREN", "LBRACE", "RBRACE",
    "LBRACKET", "RBRACKET", "COMMA", "DECIMAL", 
    "COLON", "SCOLON", "ADD", "SUB", "STAR", "DIV", 
    "EQUAL", "POWER", "GTHAN", "LTHAN",
    
    "AND", "OR", "NOT", "GOTO", "UNTIL", "IF", "ELSE",
    "RETURN",
    
    "NUMBER", "STRING", "DEF_FN", "DEF_CLS",
    
    "EQUIVALENT", "GTHAN_OR_EQUAL_TO",
    "LTHAN_OR_EQUAL_TO", "NOT_EQUAL_TO",
    "INHERIT", "METADATA", "INIT",
    
    "ADD_EQ", "SUB_EQ", "STAR_EQ", "DIV_EQ",
    
    "IDENTIFIER", "COMMENT",
    
    "PROGRAM", "MEMORY", "EXPRESSION", "ASSIGNMENT", 
    "FUNCTION_CALL", "ARGUMENTS", "ID_LINKER", "FRACTIONAL",
    "SCOPE", "STATEMENT", "CONDITION", "GTOET", "LTOET",
    "LOOP", "LIST", "LIST_ACC", "OPERATION", "CLASS", "INHERITS",
    "META_SCOPE"

};

const char *cmdt_names[] = {
    "_ADD", "_SUB", "_MUL", "_DIV",
    
    "_PUSH", "_CALL", "_STO", "_ID",
    
    "_LITERAL",
    
    "SPLIT"
};

const char *utypes[] = {
    "NODE", "NODES"
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
            enum _TYPE_ type;
            struct TAG *nodes;
            int nodeln;
        } Nodes;
        
    } UNION;
    
} NodeMaster;

/*typedef struct
{
    enum _DATATYPE_ dtype;
    union
    {
        char *string;
        int *inum;
        double *fnum;
    } val;
    
} ARG;

typedef struct 
{
    enum _COMMAND_ cmdtype;
    Arg *args;
    int argln;
    size_t argsz;
} CMD;*/

typedef struct 
{
    enum _COMMAND_ cmdt;
    int argln;
    char *val;
} CMD;
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
    // forgot why i even put this here...
    // i should comment wayyyy more often
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


