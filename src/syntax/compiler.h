#ifndef COMPILER_H
#define COMPILER_H

void acmd(CMD *cmds, size_t *cmdsz, int *cmdln, CMD obj);
CMD spawn(enum _COMMAND_ cmdtype, int _argln, char *token);

void traverse(NodeMaster *stack, enum _TYPE_ ptype, CMD* cmds, size_t *cmdsz, int *cmdln);

CMD *compile(NodeMaster *stack);

#endif
