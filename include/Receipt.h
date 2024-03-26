#include <stdbool.h>
#include <Models.h>

#ifndef __RECEIPT
#define __RECEIPT

// Reads a tesco receipt email, cleans the data and makes a receipt object out of it
Receipt *LoadReceipt(const char *);

#endif