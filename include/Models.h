#ifndef __MODELS
#define __MODELS

typedef struct
{
    char *description;
    float price;
    int quantity;
} Item;

typedef struct
{
    Item **items;
    float total;
    int count;
} Receipt;

#endif