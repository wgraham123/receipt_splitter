#include <stdbool.h>
#include <Models.h>

#ifndef __RECEIPT
#define __RECEIPT

// Reads a tesco receipt email, cleans the data and makes a receipt object out of it
void FreeReceiptData(char **);


bool IsCleanChar(char);
bool CleanString(char **);
bool CleanData(char ***);
bool ReadData(char ***, const char *);
bool MakeReceipt(char ***, Receipt **);
bool LoadReceipt(const char *, Receipt *);

#endif