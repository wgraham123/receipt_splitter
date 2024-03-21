#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <HelperFunctions.h>

size_t StringLength(char *str)
{
    if (str == NULL || str[0] == '\0')
        return 0;

    size_t str_length = 0;
    for (size_t i = 0; str[i] != '\0'; i++)
        str_length = i;

    return str_length + 1;
}

bool StringEquals(char *str1, char *str2)
{
    if (str1 == NULL && str2 == NULL)
        return true;

    if (str1 == NULL || str2 == NULL)
        return false;

    size_t str1_length = StringLength(str1);

    size_t str2_length = StringLength(str2);

    if (str1_length != str2_length)
        return false;

    for (size_t i = 0; i < str1_length; i++)
        if (str1[i] != str2[i])
            return false;

    return true;
}

int ParseInt(char *str)
{
    // Find the point in the string where the number begins
    size_t start = 0;
    while ((str[start] < 48 || str[start] > 57))
    {
        start += 1;
        // If we reached the end of the string without finding numbers, return -1
        if (str[start] == '\0')
        {
            printf("[ERROR] - No numbers found in %s\n", str);
            return -1;
        }
    }

    // extract just the numerical data from the str
    char *num_str = (char *)malloc((1 + StringLength(str)) * sizeof(char));
    for (size_t i = 0; str[i + start] >= 48 && str[i + start] <= 57; i++)
    {
        num_str[i] = str[i + start];
    }

    size_t digits = StringLength(num_str);
    unsigned int num = 0;
    for (size_t i = 0; num_str[i] != '\0'; i++)
    {
        num += (int)(num_str[i] - 48) * (Power(10, (digits - i) - 1));
    }

    free(num_str);
    return num;
}

float ParseFloat(char *str)
{
    size_t decimal_index = 0;
    while (str[decimal_index] != 46)
    {
        decimal_index += 1;
        if (str[decimal_index] == '\0')
        {
            printf("[ERROR] - No decimal point found in %s\n", str);
            return -1.0;
        }
    }

    char *num_str = (char *)malloc((1 + StringLength(str)) * sizeof(char));
    for (size_t i = 0; i < decimal_index; i++)
    {
        num_str[i] = str[i];
    }

    int num = ParseInt(num_str);

    char *decimal_str = (char *)malloc((1 + StringLength(str)) * sizeof(char));
    for (size_t i = decimal_index; str[i] != '\0'; i++)
    {
        decimal_str[i - decimal_index] = str[i];
    }

    float decimal = (float)ParseInt(decimal_str);

    while (decimal > 1)
    {
        decimal /= 10;
    }

    float result = num + decimal;

    free(num_str);
    free(decimal_str);

    return result;
}