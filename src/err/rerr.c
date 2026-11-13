#include <stdlib.h>
#include <stdio.h>


void raise_err(char *code)
{
    printf("Error: %s\n" code);
}

// raise unexpected token error
void raise_uterr()
{
    raise_err("Unexpected token");
}