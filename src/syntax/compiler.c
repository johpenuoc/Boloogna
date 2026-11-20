#include <stdlib.h>
#include <stdio.h>

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
    *cmdsz += sizeof(obj);
    cmds[*cmdln] = obj;
    (*cmdln)++;
    
}

CMD setcmd(enum _COMMAND_ _cmdtype, ARG *_args, int _argln, size_t *_argsz)
{
    CMD cmd;
    cmd.cmdtype = _cmdtype;
    cmd.argln = *_argln;
    cmd.argsz = *_argsz;
    cmd.args = _args;
    
    return cmd;
}

CMD spawn(enum _COMMAND_ _cmdtype, int _argln, char *token)
{
    /*CMD cmd;
    cmd.cmdtype = _cmdtype;
    *cmd.argln = 0;
    *cmd.argsz = 0;
    cmd.args = NULL;*/
    
    CMD cmd;
    cmd.cmdtype = _cmdtype;
    *cmd.argln = _argln;
    cmd.val = token;
    
    return cmd;
}

void traverse(NodeMaster *stack, int *depth, CMD* cmds, size_t *cmdsz, int *cmdln)
{
    if (stack->utype == NODE)
        {
            const enum _TYPE_ STYPE = stack->UNION.Node.type;
            char *val = stack->UNION.Node.stor;
            switch (STYPE)
                {
                    case EQUAL:
                        {
                            CMD sto = spawn(STO, 2, val);
                            acmd(cmds, cmdsz, cmdln, sto);
                        }
                        break;
                }
        }
    else
        {
            NodeMaster nodes = *stack;
            const enum _TYPE_ TYPE = nodes.UNION.Nodes.type;
            
            switch (TYPE) {
                case EXPRESSION:
                    break;
                case ASSIGNMENT:
                    
                    break;
                case FUNCTION_CALL:
    
                    break;
                case ARGUMENTS:
    
                    break;
                case ID_LINKER:
    
                    break;
                case FRACTIONAL:
    
                    break;
                case SCOPE:
    
                    break;
                case STATEMENT:
    
                    break;
                case CONDITION:
    
                    break;
                case GTOET:
    
                    break;
                case LTOET:
    
                    break;
                case LOOP:
    
                    break;
                case LIST:
    
                    break;
                case LIST_ACC:
    
                    break;
                case OPERATION:
    
                    break;
                case CLASS:
    
                    break;
                case INHERITS:
    
                    break;
                case META_SCOPE:
    
                    break;
                default:
    
                    break;
                } 
            
            (*depth)++;
            for (int i = 0; i < stack->UNION.Nodes.nodeln; ++i)
                {
                    NodeMaster node_s = stack->UNION.Nodes.nodes[i];
                    traverse(&node_s, depth);
                }
            (*depth)--;
        }
}

NodeMaster compile(NodeMaster *stack)
{
    
}