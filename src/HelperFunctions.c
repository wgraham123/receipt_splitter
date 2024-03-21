#include <stdlib.h>
#include <stdio.h>
#include <StringFunctions.h>

size_t Power(int base, int pow)
{
    if (pow < 0)
    {
        printf("[ERROR] - No negative powers!\n");
        return 0;
    }
    if (pow == 0)
    {
        return 1;
    }
    int result = 1;
    for (int i = 0; i < pow; i++)
    {
        result *= base;
    }
    return result;
}

int GetIndex(char *needle, char ***haystack)
{
    if (needle == NULL || haystack == NULL)
    {
        printf("[ERROR] - needle or haystack is NULL pointer\n");
        return -1;
    }

    for (size_t i = 0; (*haystack)[i] != NULL; i++)
    {
        if (StringEquals((*haystack)[i], needle))
        {
            return i;
        }
    }
    printf("[ERROR] - %s could not be found in haystack\n", needle);
    return -1;
}
