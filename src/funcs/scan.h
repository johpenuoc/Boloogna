#ifndef SCAN_H
#define SCAN_H

#include <stdio.h>

#include "coms/common.h"


List preprocessor(FILE *file);
List tokenize(List l);

#endif