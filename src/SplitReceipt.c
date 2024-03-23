#include <stdio.h>
#include <stdlib.h>

#include <StringFunctions.h>
#include <Models.h>

#define MAX_INPUT_LENGTH 64

void FreeSplitNames(char **split_names)
{
    if (split_names == NULL)
        return;

    for (int i = 0; split_names[i] != NULL; i++)
        free(split_names[i]);
    
    free(split_names);
    return;
}

void GatherNames(char ***split_names)
{

    char names[MAX_INPUT_LENGTH];
    printf("Please enter the names of the splitees as a comma separated list\n");
    scanf("%s", names);

    if(!Split(names, ',', split_names))
    {
        FreeSplitNames(*split_names);
        return;
    }

    return;
}

void SplitReceipt(Receipt *receipt)
{
    char **names;
    GatherNames(&names);

    for (int i = 0; names[i] != NULL; i++)
        printf("%s\n", names[i]);

    FreeSplitNames(names);
    return;
}