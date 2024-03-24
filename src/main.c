#include <stdio.h>
#include <stdlib.h>

#include <SplitReceipt.h>
#include <Receipt.h>
#include <Models.h>
#include <UserInput.h>

#define MAX_NUM_ITEMS 100

void FreeReceipt(Receipt *receipt)
{
    if (receipt == NULL)
        exit(EXIT_FAILURE);

    int i = 0;
    for (int i = 0; receipt->items[i] != NULL; i++)
    {
        free(receipt->items[i]->description);
        free(receipt->items[i]);
    }

    free(receipt->items);

    return;
}

void FreePeople(Person ***people)
{
    if (people == NULL)
        exit(EXIT_FAILURE);

    for (int i = 0; (*people)[i] != NULL; i++)
    {
        free((*people)[i]->name);
        FreeReceipt((*people)[i]->receipt);
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
        Receipt receipt = {
            .items = (Item **)malloc(MAX_NUM_ITEMS * sizeof(Item *)),
            .count = 0,
            .total = 0.0};

        LoadReceipt(argv[1], &receipt);

        for (int i = 0; receipt.items[i] != NULL; i++)
            printf("\nDescription: %s\nQuantity: %.1f\nPrice: £%.2f\n", receipt.items[i]->description, receipt.items[i]->quantity, receipt.items[i]->price);

        printf("\nReceipt Total: £%.2f\n", receipt.total);

        // TODO: Should probably move the name gathering out to here, or in a standalone file.
        Person **people = GatherNamesFromUser();
        if (people == NULL){
            FreeReceipt(&receipt);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; people[i] != NULL; i++)
        {
            printf("%.1f\n", people[i]->receipt->count);
        }

        SplitReceipt(&receipt, &people);

        FreeReceipt(&receipt);
        FreePeople(&people);
        return 0;
    }

    printf("Incorrect number of arguments!\n");
    return -1;
}