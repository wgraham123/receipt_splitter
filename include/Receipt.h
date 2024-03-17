#include <stdbool.h>
#include <Models.h>

#ifndef __RECEIPT
#define __RECEIPT

// Reads a tesco receipt
bool StringEquals(char *, char *);
bool IsStinkyChar(char);
void CleanLine(char *);
Receipt *LoadReceipt(const char *);
#endif