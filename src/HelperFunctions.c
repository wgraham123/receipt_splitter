#include <stdlib.h>
#include <StringFunctions.h>

double Abs(double a)
{
    if (a < 0)
        return -a;

    return a;
}

double Power(double base, int pow)
{
    if (pow < 0)
        return -1;
    
    if (pow == 0)
        return 1;

    double result = 1.0;
    for (int i = 0; i < pow; i++)
        result *= base;

    return result;
}

int GetIndex(char *needle, char **haystack)
{
    if (needle == NULL || haystack == NULL)
        return -1;


    for (int i = 0; haystack[i] != NULL; i++)
        if (StringEquals(haystack[i], needle))
            return i;

    return -1;
}
