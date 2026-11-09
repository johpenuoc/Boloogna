#ifndef SCAN_H
#define SCAN_H

#include <stdio.h>

#include "coms/common.h"


List preprocessor(FILE *file);
void incr(char *curr, int *len, char indv);
List tokenize(List l);

#endif