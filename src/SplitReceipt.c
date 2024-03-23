#include <stdio.h>
#include <stdlib.h>

#include <Models.h>

void GatherNames()
{

    char *names;
    printf("Please enter the names of the splitees as a comma separated list\n");
    scanf("%s", names);

    

    return;
}

void SplitReceipt(Receipt *receipt)
{

    char *name;

    printf("Enter a name!\n");
    scanf("%s", name);
    printf("You entered %s\n", name);

    return;
}