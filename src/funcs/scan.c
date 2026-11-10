#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "coms/common.h"


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

// use after string-token fix
char *incr(char *curr, int *len, char indv)
{
    curr[(*len) - 1] = indv;
    (*len)++;
    curr = realloc(curr, sizeof(char) * (*len));
    
    return curr;
}

void shift_down(Token *tokens, int start, int jump, int tokens_length)
{
    // not sure why it has to be 'start - 1'\
    // not gonna bother knowing, just let it be
    for (int i = start - 1; i < tokens_length - jump; ++i)
        {
            tokens[i] = tokens[i + jump];
        }
    
    tokens = realloc(tokens, sizeof(Token) * tokens_length - jump);
}

List tokenize(FILE *file)
{
    List l = preprocessor(file);
    
    Token *tokens = malloc(sizeof(Token) * 1);
    int token_cnt = 1;
    
    char *curr = malloc(sizeof(char) * 1);
    int len = 1;
    int *len_ptr = &len;
    
    bool in_mode = false;
    char mode_type;
    
    char *indv = (char *)l.element;
    
    for (int i = 0; i < l.length; ++i)
        {   
            if (!in_mode && (indv[i] == '"' || indv[i] == '#' || indv[i] == '\''))
                {
                    in_mode = true;
                    mode_type = indv[i];
                    
                    curr = curr = incr(curr, len_ptr, indv[i]);
                }
            else if (in_mode && indv[i] == mode_type && indv[i - 1] != '\\')
                {
                    in_mode = false;
                    curr = incr(curr, len_ptr, indv[i]);
                }
                
            else if (!in_mode && indv[i] == ' ' && indv[i - 1] != ' ')
                {    
                    curr[(*len_ptr) - 1] = '\0';  
                    tokens[token_cnt - 1].stor = curr;
                    tokens[token_cnt - 1].len = (*len_ptr) - 1;
                    
                    if (strcmp(curr, "(") == 0) tokens[token_cnt - 1].type = LPAREN;
                    else if (strcmp(curr, ")") == 0) tokens[token_cnt - 1].type = RPAREN;
                    else if (strcmp(curr, "{") == 0) tokens[token_cnt - 1].type = LBRACE;
                    else if (strcmp(curr, "}") == 0) tokens[token_cnt - 1].type = RBRACE;
                    else if (strcmp(curr, ",") == 0) tokens[token_cnt - 1].type = COMMA;
                    else if (strcmp(curr, ".") == 0) tokens[token_cnt - 1].type = DECIMAL;
                    else if (strcmp(curr, ":") == 0) tokens[token_cnt - 1].type = COLON;
                    else if (strcmp(curr, ";") == 0) tokens[token_cnt - 1].type = SCOLON;
                    else if (strcmp(curr, "+") == 0) tokens[token_cnt - 1].type = ADD;
                    else if (strcmp(curr, "-") == 0) tokens[token_cnt - 1].type = SUB;
                    else if (strcmp(curr, "*") == 0) tokens[token_cnt - 1].type = STAR;
                    else if (strcmp(curr, "/") == 0) tokens[token_cnt - 1].type = DIV;
                    else if (strcmp(curr, "=") == 0) tokens[token_cnt - 1].type = EQUAL;
                    else if (strcmp(curr, "^") == 0) tokens[token_cnt - 1].type = POWER;
                    else if (strcmp(curr, ">") == 0) tokens[token_cnt - 1].type = GTHAN;
                    else if (strcmp(curr, "<") == 0) tokens[token_cnt - 1].type = LTHAN;
                    
                    else if (strcmp(curr, "==") == 0) tokens[token_cnt - 1].type = EQUIVALENT;
                    else if (strcmp(curr, ">=") == 0) tokens[token_cnt - 1].type = GTHAN_OR_EQUAL_TO;
                    else if (strcmp(curr, "<=") == 0) tokens[token_cnt - 1].type = LTHAN_OR_EQUAL_TO;
                    
                    else
                        {
                            if (only_numbers(curr, (*len_ptr) - 1)) tokens[token_cnt - 1].type = NUMBER;
                            else if (curr[0] == '#' && curr[(*len_ptr) - 2] == '#') tokens[token_cnt - 1].type = COMMENT;
                            else if (curr[0] == '"' && curr[(*len_ptr) - 2] == '"') tokens[token_cnt - 1].type = STRING;
                            else
                                {
                                    if (strcmp(curr, "&") == 0) tokens[token_cnt - 1].type = AND;
                                    else if (strcmp(curr, "|") == 0) tokens[token_cnt - 1].type = OR;
                                    else if (strcmp(curr, "!") == 0) tokens[token_cnt - 1].type = NOT;
                                    else if (strcmp(curr, "goto") == 0) tokens[token_cnt - 1].type = GOTO;
                                    else if (strcmp(curr, "until") == 0) tokens[token_cnt - 1].type = UNTIL;
                                    else if (strcmp(curr, "if") == 0) tokens[token_cnt - 1].type = IF;
                                    else if (strcmp(curr, "else") == 0) tokens[token_cnt - 1].type = ELSE;
                                    else
                                        {
                                            tokens[token_cnt - 1].type = IDENTIFIER;
                                        }
                                }
                        }
                        
                    curr = malloc(sizeof(char) * 1);
                    *len_ptr = 1;
                    
                    token_cnt++;
                    tokens = realloc(tokens, sizeof(Token) * token_cnt);
                }
            else
                {
                    if ((indv[i] != ' ' && !in_mode) || in_mode) 
                        {
                            curr = incr(curr, len_ptr, indv[i]);;
                        }
                }
        }
        
    free(curr);
    free(l.element);
    
    
    // after tokenizing, there are still some things to clear up
    // we firstly need to combine 'DECIMAL's with 'NUMBER's. 
    // this will be a simple gathering.
    
    // a number may start with a 'DECIMAL', '.', provided that
    // the previous token was non-terminal
    int token_cnt_change;
    char *number;
    int incr = 0;
    for (int i = 1; i < token_cnt - 2; ++i)
        {
            enum _TYPE_ currt = tokens[i].type;
            enum _TYPE_ prevt = tokens[i - 1].type;
            enum _TYPE_ nextt = tokens[i + 1].type;
            
            if (prevt == NUMBER && currt == DECIMAL && nextt == NUMBER)
                {
                    int len = strlen(tokens[i - 1].stor);
                    number = malloc(sizeof(char) * len);
                
                    for (int j = 0; j < strlen(tokens[i - 1].stor); ++j)
                        {
                            number[j] = tokens[i - 1].stor[j];
                            
                            //printf("%s\n", number);
                        }
                    
                    len++;
                    number = realloc(number, sizeof(char) * len);
                    number[len - 1] = '.';
                    
                    int str2_len = strlen(tokens[i + 1].stor);
                    number = realloc(number, sizeof(char) * (len + str2_len));
                    
                    for (int j = 0; j < str2_len; ++j)
                        {
                            number[len + j] = tokens[i + 1].stor[j];
                        }
                    len += str2_len;
                    number[len] = '\0';
                    
                    tokens[i - 1].stor = realloc(tokens[i - 1].stor, sizeof(char) * len);
                    strcpy(tokens[i - 1].stor, number);
                    shift_down(tokens, i + 1, 2, token_cnt);
                    token_cnt -= 2;
                    
                    free(number);
                    
                }
            
            
            
        }
        
    
    return (List){ .element = tokens, .length = token_cnt};
}