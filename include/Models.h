#ifndef __MODELS
#define __MODELS

struct Item
{
    char *description;
    float price;
    int quantity;
};

typedef struct Item Item;

struct Receipt
{
    Item **items;
    float total;
};

typedef struct Receipt Receipt;

#endif