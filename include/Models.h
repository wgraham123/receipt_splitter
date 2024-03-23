#ifndef __MODELS
#define __MODELS

typedef struct
{
    char *description;
    double price;
    double quantity;
} Item;

typedef struct
{
    Item **items;
    double total;
    double count;
} Receipt;

typedef struct
{
    Receipt *receipt;
    char *name;
} Person;

#endif