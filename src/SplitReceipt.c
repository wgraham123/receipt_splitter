#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <Models.h>
#include <StringFunctions.h>

bool AddItemToPersonsReceipt(Item *item, double share, char *splitee, Person ***people)
{
    for (int i = 0; (*people)[i] != NULL; i++)
    {
        if (StringEquals((*people)[i]->name, splitee))
        {
            AppendItem(&((*people)[i]->receipt->items), *item);
            (*people)[i]->receipt->count += item->quantity * share;
            (*people)[i]->receipt->total += item->price * share;
            return true;
        }
    }
    return false;
}

bool SpliteesAreValid(char **splitees, Person **people)
{
    return true;
}

void SplitItem(Item *item, Person ***people)
{
    char **splitees;
    
    while(1)
    {
        splitees = GetSpliteesFromUser();
        if (SpliteesAreValid(splitees, *people))
            break;
    }

    int num_splitees = 0;
    while (splitees[num_splitees] != NULL)
        num_splitees += 1;

    printf("\nSplitees: %s", splitees[0]);
    for (int i = 1; splitees[i] != NULL; i++)
        printf(", %s", splitees[i]);
    printf("\nNum = %i\n", num_splitees);

    double equal_sharing = 1.0 / (double)num_splitees;

    for (int i = 0; splitees[i] != NULL; i++)
        AddItemToPersonsReceipt(item, equal_sharing, splitees[i], people);
    
    return;
}

void SplitReceipt(Receipt *receipt, Person ***people)
{
    printf("\n\nFor each item, respond with a comma separated list of the names of the people to include in this split");

    for (int i = 0; receipt->items[i] != NULL; i++)
    {
        printf("\n\nItem %i / %i: %i x %s\n", i + 1, (int)receipt->count, (int)receipt->items[i]->quantity, receipt->items[i]->description);
        printf("Price = £%.2f\n", receipt->items[i]->price);
        SplitItem(receipt->items[i], people);
    }
    return;
}