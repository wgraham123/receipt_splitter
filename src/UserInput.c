#include <stdlib.h>
#include <stdio.h>

#include <Models.h>
#include <StringFunctions.h>

#define MAX_INPUT_LENGTH 128

void FreeSplitNames(char **split_names)
{
    if (split_names == NULL)
        return;

    for (int i = 0; split_names[i] != NULL; i++)
        free(split_names[i]);

    free(split_names);
    return;
}

Person **GatherNamesFromUser()
{

    char names[MAX_INPUT_LENGTH];
    printf("Please enter the names of the splitees as a comma separated list\n");
    scanf("%s", names);
    char **split_names;

    if (!Split(names, ',', &split_names))
    {
        FreeSplitNames(split_names);
        return NULL;
    }

    int num_people = 0;
    while (split_names[num_people] != NULL)
        num_people += 1;

    Person **people = (Person **)malloc((num_people + 1) * sizeof(Person *));
    if (people == NULL)
    {
        FreeSplitNames(split_names);
        return NULL;
    }

    for (int i = 0; i < num_people; i++)
        people[i] = (Person *)malloc(sizeof(Person));

    for (int i = 0; people[i] != NULL; i++)
    {
        // Copy name into person name
        people[i]->name = (char *)malloc((1 + StringLength(split_names[i])) * sizeof(char));
        if (people[i]->name == NULL)
        {
            FreeSplitNames(split_names);
            return NULL;
        }

        for (int j = 0; split_names[i][j] != '\0'; j++)
            people[i]->name[j] = split_names[i][j];
        
        // set up person receipt
        people[i]->receipt = (Receipt *)malloc(sizeof(Receipt));
        Receipt r = {
            .items = NULL,
            .count = 0.0,
            .total = 0.0
        };
        people[i]->receipt = &r;
    }

    return people;
}