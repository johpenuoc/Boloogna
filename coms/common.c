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

// these are _TYPE_s which are not found in code but are found after
// stack generation
enum _STACK_TYPE_
{
    PROGRAM, MEMORY, EXPRESSION, ASSIGNMENT, 
    FUNCTION_CALL, ARGUMENTS, ID_LINKER, FRACTIONAL,
    SCOPE, STATEMENT, CONDITION, _IF, _ELSE, 
    _AND, _OR, _NOT, _GTOET, _LTOET, _DEF_FN,
    LOOP, ADDEQ, SUBEQ, STAREQ, DIVEQ, LIST
};

enum _DATATYPE_
{
    _INT_, _STRING_, _CHAR_, _BOOL_,
    _FLOAT_
};

const char *type_names[] = {
    "LPAREN", "RPAREN", "LBRACE", "RBRACE",
    "LBRACKET", "RBRACKET",
    "COMMA", "DECIMAL", "COLON", "SCOLON", 
    "ADD", "SUB", "STAR", "DIV", "EQUAL", 
    "POWER", "GTHAN", "LTHAN",
    
    "AND", "OR", "NOT", "GOTO", "UNTIL", "IF", "ELSE",
    
    "NUMBER", "STRING", "DEF_FN",
    
    "EQUIVALENT", "GTHAN_OR_EQUAL_TO", "LTHAN_OR_EQUAL_TO",
    "NOT_EQUAL_TO", "ADD_EQ", "SUB_EQ", "STAR_EQ", "DIV_EQ",
    
    "IDENTIFIER", "COMMENT", "PROGRAM", "MEMORY"

};

const char *stack_type_names[] = {
    "PROGRAM", "MEMORY", "EXPRESSION", "ASSIGNMENT", 
    "FUNCTION", "ARGUMENTS", "ID_LINKER", "FRACTIONAL",
    "SCOPE", "STATEMENT", "CONDITION", "IF", "ELSE",
    "AND", "OR", "NOT", "GTOET", "LTOET", "DEF_FN",
    "ULOOP", "PLUSEQ", "MINUSEQ", "STAREQ", "DIVEQ"
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
            enum _STACK_TYPE_ type;
            struct TAG *nodes;
            int nodeln;
        } Nodes;
        
    } UNION;
    
} NodeMaster;

// ==============================
// ==============================

enum _TYPE_ _type_to_stack_type(enum _TYPE_ type)
{
    if (type == AND) return _AND;
    else if (type == OR) return _OR;
    else
        {
            printf("Doesn't exist\n");
            return 1;
        }
}

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


