#include <stdbool.h>

#ifndef __STRING_FUNCTIONS
#define __STRING_FUNCTIONS

int StringLength(char *);
bool StringEquals(char *, char *);
int ParseDigit(char);
int ParseInt(char *);
double ParseFloat(char *);
bool Split(char *, char, char ***);

#endif