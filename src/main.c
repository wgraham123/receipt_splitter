#include <stdio.h>
#include <stdlib.h>

#include <SplitReceipt.h>
#include <Receipt.h>
#include <Models.h>

#define MAX_NUM_ITEMS 100

void FreeReceipt(Receipt *receipt)
{
    if (receipt == NULL)
        exit(EXIT_FAILURE);

    size_t i = 0;
    for (size_t i = 0; receipt->items[i] != NULL; i++)

        free(receipt->items[i]->description);
        free(receipt->items[i]);

    free(receipt->items);
    
    return;
}

int main(int argc, char** argv){

    if (argc == 2) {

        Receipt receipt = {
            .items = (Item **)malloc(MAX_NUM_ITEMS * sizeof(Item *)),
            .count = 0,
            .total = 0.0
        };

        LoadReceipt(argv[1], &receipt);

        for (int i = 0; receipt.items[i] != NULL; i++)
            printf("\nDescription: %s\nQuantity: %.1f\nPrice: £%.2f\n", receipt.items[i]->description, receipt.items[i]->quantity, receipt.items[i]->price);

        printf("\nReceipt Total: £%.2f\n", receipt.total);
        
        SplitReceipt(&receipt);

        FreeReceipt(&receipt);
        return 0;
    }

    printf("Incorrect number of arguments!\n");
    return -1;
}