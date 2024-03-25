#include <stdio.h>
#include <stdlib.h>

#include <SplitReceipt.h>
#include <Receipt.h>
#include <Models.h>
#include <UserInput.h>

#define MAX_NUM_ITEMS 100

void FreeReceiptItems(Item **items)
{
    if (items == NULL)
        return;
    
    for (int i = 0; items[i] != NULL; i++)
    {
        if (items[i]->description != NULL)
            free(items[i]->description);

        free(items[i]);
    }

    return;
}

void FreePeople(Person ***people)
{
    if (*people == NULL)
        exit(EXIT_FAILURE);

    for (int i = 0; (*people)[i] != NULL; i++)
    {
        if ((*people)[i]->name != NULL)
            free((*people)[i]->name);

        if ((*people)[i]->receipt != NULL)
            FreeReceiptItems((*people)[i]->receipt->items);

        free((*people)[i]);
    }

    free((*people));

    return;
}

int main(int argc, char **argv)
{

    if (argc == 2)
    {
        // TODO: shouldnt allocate MAX_NUM_ITEMS pointers, i should just leave it as NULL and then
        // do it dynamically in LoadReceipt.

        Receipt *receipt = LoadReceipt(argv[1]);

        for (int i = 0; receipt->items[i] != NULL; i++)
            printf("\nDescription: %s\nQuantity: %.1f\nPrice: £%.2f\n", receipt->items[i]->description, receipt.items[i]->quantity, receipt.items[i]->price);

        printf("\nReceipt Total: £%.2f\n", receipt->total);

        // TODO: Should probably move the name gathering out to here, or in a standalone file.
        Person **people = GatherNamesFromUser();
        if (people == NULL)
        {
            FreeReceiptItems(receipt.items);
            exit(EXIT_FAILURE);
        }

        SplitReceipt(&receipt, &people);

        FreeReceiptItems(receipt.items);
        FreePeople(&people);
        return 0;
    }

    printf("Incorrect number of arguments!\n");
    return -1;
}