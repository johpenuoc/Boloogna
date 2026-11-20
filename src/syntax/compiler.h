#ifndef COMPILER_H
#define COMPILER_H

void acmd(CMD *cmds, size_t *cmdsz, int *cmdln, CMD obj);
CMD setcmd(enum _COMMAND_ _cmdtype, ARG *_args, int _argln, size_t *_argsz);
CMD spawn(enum _COMMAND_ _cmdtype);

void traverse(NodeMaster *stack, int *depth, CMD* cmds, size_t *cmdsz, int *cmdln);

NodeMaster compile(NodeMaster *stack);

#endif
