#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "coms/common.h"

//STO , a , MUL , LOAD , y: MUL , FNCALL , x: LITERAL , 0.5:::

/*
STACK: [ASSIGNMENT]
                NODE: [EQUAL]-------->[=]
                NODE: [IDENTIFIER]--->[a]
                STACK: [EXPRESSION]
                        STACK: [OPERATION]
                                NODE: [STAR]--------->[*]
                                NODE: [IDENTIFIER]--->[y]
                                STACK: [EXPRESSION]
                                        STACK: [OPERATION]
                                                NODE: [STAR]--------->[*]
                                                STACK: [FUNCTION_CALL]
                                                        NODE: [IDENTIFIER]--->[func]
                                                        STACK: [ARGUMENTS]
                                                                STACK: [EXPRESSION]
                                                                        NODE: [IDENTIFIER]--->[x]
                                                NODE: [NUMBER]------->[0.5]
*/

void acmd(CMD *cmds, size_t *cmdsz, int *cmdln, CMD obj)
{
    (*cmdsz) += sizeof(obj);
    cmds = realloc(cmds, *cmdsz);
    cmds[*cmdln] = obj;
    (*cmdln)++;
    
    //printf("%s -> ", cmdt_names[cmds[(*cmdln) - 1].cmdt]);
}

CMD spawn(enum _COMMAND_ cmdtype, int _argln, char *token)
{
    /*CMD cmd;
    cmd.cmdtype = _cmdtype;
    *cmd.argln = 0;
    *cmd.argsz = 0;
    cmd.args = NULL;*/
    
    CMD cmd;
    cmd.cmdt = cmdtype;
    cmd.argln = _argln;
    cmd.val = malloc(strlen(token) + 1);
    strcpy(cmd.val, token);
    
    return cmd;
}

/*CMD spawn_splitter()
{
    CMD split;
    split.cmdt = SPLIT;
    split.argln = 0;
    split.val = ":";
    
    return split;
}*/

//CMD* cmds, size_t *cmdsz, int *cmdln
void traverse(NodeMaster *stack, enum _TYPE_ ptype, CMD* cmds, size_t *cmdsz, int *cmdln)
{
    if (stack->utype == NODE)
        {
            const enum _TYPE_ STYPE = stack->UNION.Node.type;
            char *val = stack->UNION.Node.stor;
            enum _COMMAND_ ctype;
            
            switch (STYPE)
                {
                    case EQUAL:
                        {
                            ctype = _STO;
                        }
                        break;
                    case IDENTIFIER:
                        {
                            if (ptype == FUNCTION_CALL) ctype = _CALL;
                            else ctype = _ID;
                        }
                        break;
                    case ADD:
                        {
                            ctype = _ADD;
                        }
                        break;
                    case SUB:
                        {
                            ctype = _SUB;
                        }
                        break;
                    case DIV:
                        {
                            ctype = _DIV;
                        }
                        break;
                    case STAR:
                        {
                            ctype = _MUL;
                        }
                        break;
                    case NUMBER:
                        {
                            ctype = _LITERAL;
                        }
                        break;
                    default:
                        //printf("default: %s\n", type_names[stack->UNION.Nodes.type]);
                        break;
                    
                }
            CMD c = spawn(ctype, 2, val);
            acmd(cmds, cmdsz, cmdln, c);
            printf("size: %d\n", sizeof(cmds[(*cmdln) - 1]));
        }
    else
        {      
            //(*depth)++;
            for (int i = 0; i < stack->UNION.Nodes.nodeln; ++i)
                {
                    enum _TYPE_ ptype = stack->UNION.Nodes.type;
                    NodeMaster node_s = stack->UNION.Nodes.nodes[i];
                    traverse(&node_s, ptype, cmds, cmdsz, cmdln);
                }
            //(*depth)--;
        }
}

CMD *compile(NodeMaster *stack)
{
    size_t cmdsz = 0;
    int cmdln = 0;
    CMD *cmds;
    traverse(stack, PROGRAM, cmds, &cmdsz, &cmdln);
    
    
    for (int i = 0; i < cmdln - 1; ++i)
        {
            printf("id: %d\n", i);
            CMD cmd = *cmds;
            
            printf("%s ", cmdt_names[cmd.cmdt]);
        }
        
    return cmds;
}