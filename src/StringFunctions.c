#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <HelperFunctions.h>

#define FLOAT_TOL 0.001

int StringLength(char *str)
{
    if (str == NULL || str[0] == '\0')
        return 0;

    int str_length = 0;
    while (str[str_length] != '\0')
        str_length += 1;

    return str_length;
}

bool StringEquals(char *str1, char *str2)
{
    if (str1 == NULL && str2 == NULL)
        return true;

    if (str1 == NULL || str2 == NULL)
        return false;

    int str1_length = StringLength(str1);
    int str2_length = StringLength(str2);

    if (str1_length != str2_length)
        return false;

    for (int i = 0; i < str1_length; i++)
        if (str1[i] != str2[i])
            return false;

    return true;
}

int ParseDigit(char c)
{
    if (c < 48 || c > 57)
        return -1;

    return (int)c - 48;
}

int ParseInt(char *str)
{
    if (ParseDigit(str[0]) == -1)
        return -1;

    int digits = 0;
    while (ParseDigit(str[digits]) != -1)
        digits += 1;

    int num = 0;
    for (int i = 0; ParseDigit(str[i]) != -1; i++)
        num += ParseDigit(str[i]) * (int)(Power(10.0, (digits - i) - 1));

    return num;
}

double ParseFloat(char *str)
{
    // Find index of decimal point
    int decimal_index = 0;
    while (str[decimal_index] != 46)
    {
        decimal_index += 1;
        if (str[decimal_index] == '\0')
            return -1.0;
    }

    // Parse the int up to the decimal point
    char *num_str = (char *)malloc((1 + decimal_index) * sizeof(char));
    if (num_str == NULL)
        return -1.0;

    for (int i = 0; i < decimal_index; i++)
        num_str[i] = str[i];

    int num = ParseInt(num_str);

    free(num_str);

    if (num == -1)
        return -1.0;

    // Parse the int after the decimal point, need to count leading 0s
    int leading_zeros = 0;
    while (ParseDigit(str[(decimal_index + 1) + leading_zeros]) == 0)
        leading_zeros += 1;

    char *decimal_str = (char *)malloc((StringLength(str) - decimal_index) * sizeof(char));
    for (int i = decimal_index + 1; str[i] != '\0'; i++)
        decimal_str[i - (decimal_index + 1)] = str[i];

    double decimal = (double)ParseInt(decimal_str);

    free(decimal_str);

    if (decimal == -1.0)
        return -1.0;

    while (decimal >= 1.0)
        decimal /= 10;

    decimal *= Power(0.1, leading_zeros);

    double result = (double)num + decimal;

    return result;
}

bool Split(char *str, char delim, char ***result)
{
    int str_idx = 0;
    int prev_delim_idx = -1;
    int result_idx = 0;

    // Work out how many delims there are in the string
    int num_delims = 0;
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] == delim)
            num_delims += 1;

    if (num_delims == 0)
        return false;

    // Allocate memory for the necessary number of pointers.
    (*result) = (char **)malloc((2 + num_delims) * sizeof(char *));
    if ((*result) == NULL)
        return false;

    // Split the string and store each split section
    while (str_idx < StringLength(str) + 1)
    {
        if (str[str_idx] == delim || str[str_idx] == '\0')
        {
            (*result)[result_idx] = (char *)malloc((1 + str_idx - prev_delim_idx) * sizeof(char));
            if ((*result)[result_idx] == NULL)
                return false;

            for (int i = prev_delim_idx + 1; i < str_idx; i++)
                (*result)[result_idx][i - (prev_delim_idx + 1)] = str[i];
            
            result_idx += 1;
            prev_delim_idx = str_idx;
        }
        str_idx += 1;
    }

    // Move all the strings equal to "\0" values to the back
    result_idx = 0;
    for (int i = 0; (*result)[i] != NULL; i++)
    {
        if (StringLength((*result)[i]) > 0)
        {
            char *temp = (*result)[result_idx];
            (*result)[result_idx] = (*result)[i];
            (*result)[i] = temp;
            result_idx += 1;
        }
    }

    // Free all the strings that are equal to "\0"
    for (int i = 0; (*result)[i] != NULL; i++)
    {
        if ((*result)[i][0] == '\0')
        {
            free((*result)[i]);
            (*result)[i] = NULL;
        }
    }

    return true;
}