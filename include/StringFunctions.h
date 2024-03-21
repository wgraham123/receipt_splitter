#include <stdbool.h>
#include <stdlib.h>

#ifndef __STRING_FUNCTIONS
#define __STRING_FUNCTIONS

size_t StringLength(char *);
bool StringEquals(char *, char *);
int ParseInt(char *);
float ParseFloat(char *);

#endif