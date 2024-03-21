#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <Models.h>
#include <StringFunctions.h>
#include <HelperFunctions.h>

void FreeReceiptData(char ***receipt_data)
{
    if ((*receipt_data) == NULL)
        return;

    for (size_t i = 0; (*receipt_data)[i] != NULL; i++)
        free((*receipt_data)[i]);
    free((*receipt_data));
    
    return;
}

bool IsCleanChar(char c)
{
    if (c >= 32 && c <= 126)
        return true;
    return false;
}

bool CleanString(char **dirty_string)
{
    if ((*dirty_string) == NULL){
        printf("[ERROR] - dirty_string = NULL\n");
        return false;
    }

    size_t dirty_len = StringLength((*dirty_string));

    // count clean chars between str_start_idx and str_end_idx
    size_t clean_len = 0;
    for (size_t i = 0; i < dirty_len; i++)
        if (IsCleanChar((*dirty_string)[i]))
            clean_len += 1;

    // allocate some memory for clean_len + 1 chars
    char *clean_string = (char *)malloc((1 + clean_len) * sizeof(char));
    if (clean_string == NULL)
    {
        printf("[ERROR] - Failed to allocate memory to clean_string\n");
        return false;
    }

    // fill clean_string with the clean chars from dirty_string
    size_t clean_idx = 0;
    for (size_t i = 0; i < dirty_len; i++)
    {
        if (IsCleanChar((*dirty_string)[i]))
        {
            clean_string[clean_idx] = (*dirty_string)[i];
            clean_idx += 1;
        }
    }

    if (clean_string[clean_len] != '\0')
    {
        printf("[ERROR] - clean_string overflow\n");
        return false;
    }

    // store dirty_string pointer in temp pointer
    char *temp = (*dirty_string);

    //point dirty_string at clean_string
    (*dirty_string) = clean_string;

    // free memory pointed at by temp pointer
    free(temp);

    return true;
}

bool CleanData(char ***receipt_data)
{
    printf("[LOG] - Entered CleanData()\n");
    // Count how many elements are in the receipt_data array
    size_t num_strings = 0;
    for (size_t i = 0; (*receipt_data)[i] != NULL; i++)
        num_strings = i;
    
    if (num_strings == 0)
    {
        printf("[ERROR] - num_strings = %zu\n", num_strings);
        return false;
    }

    // Clean each string in receipt data
    for (size_t i = 0; i < num_strings; i++){
        CleanString(&((*receipt_data)[i]));
    }

    // resize receipt_data, leaving out any strings with no useful data
    size_t num_valid_strings = 0;
    for (size_t i = 0; (*receipt_data)[i] != NULL; i++)
        if (StringLength((*receipt_data)[i]) != 0)
            num_valid_strings += 1;

    char **clean_data = (char **)malloc((1 + num_valid_strings) * sizeof(char *));
    if (clean_data == NULL)
    {
        printf("[ERROR] - Failed to allocate memory for clean_data\n");
        return false;
    }

    // go through receipt_data, copying strings with data to clean data as you go
    size_t clean_data_idx = 0;
    for (size_t i = 0; (*receipt_data)[i] != NULL; i++)
    {
        size_t string_length = StringLength((*receipt_data)[i]);
        if (string_length != 0)
        {
            clean_data[clean_data_idx] = (char *)malloc((1 + string_length) * sizeof(char));
            if (clean_data[clean_data_idx] == NULL)
            {
                printf("[ERROR] - Failed to allocate memory for clean_data[%zu]\n", clean_data_idx);
                return false;
            }
            for (size_t j = 0; j < string_length; j++)
                clean_data[clean_data_idx][j] = (*receipt_data)[i][j];

            clean_data_idx += 1;
        }
    }

    // move the receipt_data pointer to the clean data memory, 
    // then free all the old receipt_data memory
    char **temp = (*receipt_data);

    (*receipt_data) = clean_data;

    for (size_t i = 0; temp[i] != NULL; i++)
        free(temp[i]);
    free(temp);

    printf("[LOG] - Leaving CleanData()\n");
    return true;
}

bool ReadData(char ***receipt_data, const char *path)
{
    printf("[LOG] - Entered ReadData()\n");

    const size_t MAX_EMAIL_LINES = 10000;
    const size_t MAX_LINE_BYTES = 5000;

    // allocate memory for receipt_data pointers
    *receipt_data = (char **)malloc((1 + MAX_EMAIL_LINES) * sizeof(char *));
    if (*receipt_data == NULL)
    {
        printf("[ERROR] - Failed to allocate memory for receipt_data\n");
        return false;
    }

    // open the file and check for error
    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        printf("[ERROR] - Failed to open file %s for reading\n", path);
        return false;
    }

    // read each line from the file, storing it in receipt_data.
    char buffer[MAX_LINE_BYTES];
    for (size_t i = 0; fgets(buffer, MAX_LINE_BYTES, f) && i < MAX_EMAIL_LINES; i++)
    {
        size_t buffer_length = 0;
        for (size_t i = 0; buffer[i] != '\0'; i++)
            buffer_length = i;

        // allocate memory to store the string from buffer
        (*receipt_data)[i] = (char *)malloc((1 + buffer_length) * sizeof(char));
        if ((*receipt_data)[i] == NULL)
        {
            printf("[ERROR] - Failed to allocate memory for receipt_data[%zu]\n", i);
            return false;
        }

        // copy string from buffer to receipt_data
        for (size_t j = 0; j < buffer_length; j++)
            (*receipt_data)[i][j] = buffer[j];
    }
    fclose(f);
    printf("[LOG] - Leaving ReadData()\n");
    return true;
}

bool MakeReceipt(char ***receipt_data, Receipt **receipt)
{
    printf("[LOG] - Entered MakeReceipt()\n");

    // Find the index of the line that says "Substitutions"
    size_t substitutions_index = GetIndex("Substitutions", receipt_data);
    if (substitutions_index == -1)
        return false;

    // Find the index of the line that says "The rest of your items"
    size_t rest_of_items_index = GetIndex("The rest of your items", receipt_data);
    if (rest_of_items_index == -1)
        return false;

    // Work through the data between "Substitutions" and "The rest of your items"
    size_t item_index = 0;
    bool begin_processing = false;
    for (size_t i = substitutions_index; i < rest_of_items_index; i++)
    {
        if (StringEquals((*receipt_data)[i], "Total"))
        {
            begin_processing = true;
            continue;
        }

        if (begin_processing)
        {

            Item *item = (Item *)malloc(sizeof(Item));
            item->price = ParseFloat((*receipt_data)[i + 4]);
            item->quantity = ParseInt((*receipt_data)[i + 5]);

            item->description = (char *)malloc((1 + StringLength((*receipt_data)[i + 6])) * sizeof(char));
            for (size_t j = 0; (*receipt_data)[i + 6][j] != '\0'; j++)
                item->description[j] = (*receipt_data)[i + 6][j];

            (*receipt)->items[item_index] = item;
            item_index += 1;
            i += 7;
        }
    }


    printf("[LOG] - Leaving MakeReceipt()\n");
    return true;
}

bool LoadReceipt(const char *path, Receipt *receipt)
{
    printf("[LOG] - Entered LoadReceipt()\n");
    char **receipt_data;

    // Read the raw data from the email into receipt_data
    if (!ReadData(&receipt_data, path))
    {
        FreeReceiptData(&receipt_data);
        return false;
    }

    // Clean the raw data
    if(!CleanData(&receipt_data))
    {
        FreeReceiptData(&receipt_data);
        return false;
    }

    // Make a receipt object out of the cleaned data
    if(!MakeReceipt(&receipt_data, &receipt))
    {
        FreeReceiptData(&receipt_data);
        return false;
    }

    // for (size_t i = 0; receipt_data[i] != NULL; i++)
    //     printf("%s\n", receipt_data[i]);

    // free receipt_data as no longer needed
    FreeReceiptData(&receipt_data);

    return true;
}
