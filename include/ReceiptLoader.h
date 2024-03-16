#include <stdbool.h>

#ifndef __RECEIPT_LOADER
#define __RECEIPT_LOADER
// Reads a tesco receipt
bool Streq(char *, char *);
bool IsStinkyChar(char);
char *CleanLine(char *);
char **ReadReceipt(const char *);
#endif