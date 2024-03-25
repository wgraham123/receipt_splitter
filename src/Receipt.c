#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <Models.h>
#include <StringFunctions.h>
#include <HelperFunctions.h>

#define MAX_EMAIL_LINES 5000
#define MAX_LINE_BYTES 512
#define MAX_ITEMS 500

void FreeReceiptData(char **receipt_data)
{
    if (receipt_data == NULL)
        return;

    for (size_t i = 0; receipt_data[i] != NULL; i++)
        free(receipt_data[i]);
    free(receipt_data);

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
    if ((*dirty_string) == NULL)
    {
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

    // point dirty_string at clean_string
    (*dirty_string) = clean_string;

    // free memory pointed at by temp pointer
    free(temp);

    return true;
}

bool CleanData(char ***receipt_data)
{
    printf("[LOG] - Entered CleanData()\n");
    // Count how many elements are in the receipt_data array
    int num_strings = 0;
    while ((*receipt_data)[num_strings] != NULL)
        num_strings += 1;

    if (num_strings == 0)
        return false;
    
    // Clean each string in receipt data
    for (int i = 0; i < num_strings; i++)
        CleanString(&((*receipt_data)[i]));

    // resize receipt_data, leaving out any strings with no useful data
    int num_valid_strings = 0;
    for (int i = 0; (*receipt_data)[i] != NULL; i++)
        if (StringLength((*receipt_data)[i]) != 0)
            num_valid_strings += 1;

    char **clean_data = (char **)malloc((1 + num_valid_strings) * sizeof(char *));
    if (clean_data == NULL)
        return false;

    // go through receipt_data, copying strings with data to clean data as you go
    int clean_data_idx = 0;
    for (int i = 0; (*receipt_data)[i] != NULL; i++)
    {
        int string_length = StringLength((*receipt_data)[i]);
        if (string_length != 0)
        {
            clean_data[clean_data_idx] = (char *)malloc((1 + string_length) * sizeof(char));
            if (clean_data[clean_data_idx] == NULL)
            {
                printf("[ERROR] - Failed to allocate memory for clean_data[%zu]\n", clean_data_idx);
                return false;
            }
            for (int j = 0; j < string_length; j++)
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

bool ReadDataFromFile(char *path)
{
    printf("[LOG] - Entered ReadData()\n");

    FILE *f = fopen(path, "r");
    if (f == NULL)
        return false;

    char *data_buf[MAX_EMAIL_LINES];
    char line_buf[MAX_LINE_BYTES];
    for (int i = 0; fgets(line_buf, MAX_LINE_BYTES, f) && i < MAX_EMAIL_LINES; i++)
    {
        int line_buf_length = StringLength(line_buf);

        data_buf[i] = (char *)malloc((1 + line_buf_length) * sizeof(char));
        if (data_buf[i] == NULL)
        {
            FreeData(data_buf);
            return false;
        }

        for (int j = 0; j < line_buf_length; j++)
            data_buf[i][j] = line_buf[j];
    }
    fclose(f);

    int data_buf_length = 0;
    while(data_buf[data_buf_length] != NULL)
        data_buf_length += 1;

    char **data = (char **)malloc((1 + data_buf_length) * sizeof(char *));
    if (data == NULL)
    {
        FreeData(data_buf);
        return false;
    }

    for (int i = 0; data_buf[i] != NULL; i++)
        data[i] = data_buf[i];

    printf("[LOG] - Leaving ReadData()\n");
    return data;
}

bool ExtractSubstitutions(char **receipt_data, Item ***items_buf)
{
    int subs_index = GetIndex("Substitutions", receipt_data);
    if (subs_index == -1)
        return false;

    // Find the index of the line that says "The rest of your items"
    int roi_index = GetIndex("The rest of your items", receipt_data);
    if (roi_index == -1)
        return false;

    // Work through the data between "Substitutions" and "The rest of your items"
    int item_index = 0;
    while ((*items_buf)[item_index] != NULL)
        item_index += 1;

    bool begin_processing = false;
    for (int i = subs_index; i < roi_index; i++)
    {
        if (StringEquals(receipt_data[i], "Total"))
        {
            begin_processing = true;
            continue;
        }

        if (begin_processing)
        {

            Item *item = (Item *)malloc(sizeof(Item));
            if (item == NULL)
                return false;

            item->price = ParseFloat(receipt_data[i + 4]);
            item->quantity = (double)ParseInt(receipt_data[i + 5]);

            item->description = (char *)malloc((1 + StringLength(receipt_data[i + 6])) * sizeof(char));
            if (item->description == NULL)
            {
                free(item);
                return false;
            }

            for (int j = 0; receipt_data[i + 6][j] != '\0'; j++)
                item->description[j] = receipt_data[i + 6][j];

            (*items_buf)[item_index] = item;
            item_index += 1;
            i += 7;
        }
    }

    return true;
}

bool ExtractRestOfItems(char **receipt_data, Item ***items_buf)
{
    int roi_index = GetIndex("The rest of your items", receipt_data);
    if (roi_index == -1)
        return false;

    int vat_index = GetIndex("Items marked with an  include VAT at 20%. Items marked with  include VAT at 5%.", *receipt_data);
    if (vat_index == -1)
        return false;

    int item_index = 0;
    while ((*items_buf)[item_index] != NULL)
        item_index += 1;

    bool begin_processing = false;
    for (int i = roi_index; i < vat_index; i++)
    {
        if (StringEquals(receipt_data[i], "Total"))
        {
            begin_processing = true;
            continue;
        }

        if (begin_processing)
        {

            if (receipt_data[i][0] < 48 || receipt_data[i][0] > 57)
                continue;

            Item *item = (Item *)malloc(sizeof(Item));
            if (item == NULL)
                return false;

            item->price = ParseFloat(receipt_data[i + 3]);
            item->quantity = (double)ParseInt(receipt_data[i]);

            item->description = (char *)malloc((1 + StringLength(receipt_data[i + 1])) * sizeof(char));
            if (item->description == NULL)
            {
                free(item);
                return false;
            }

            for (int j = 0; receipt_data[i + 1][j] != '\0'; j++)
                item->description[j] = receipt_data[i + 1][j];

            (*items_buf)[item_index] = item;
            item_index += 1;
            i += 4;
        }
    }

    return true;
}

bool AddDelivery(char **data, Item ***items_buf)
{
    int delivery_index = GetIndex("Pick, pack and deliver", data);
    if (delivery_index == -1)
        return false;

    Item *item = (Item *)malloc(sizeof(Item));
    if (item == NULL)
        return false;

    item->price = ParseFloat(data[delivery_index + 1]);
    item->quantity = 1.0;

    char *delivery = "Delivery";

    item->description = (char *)malloc((1 + StringLength(delivery)) * sizeof(char));
    if (item->description == NULL)
    {
        free(item);
        return false;
    }

    for (int j = 0; delivery[j] != '\0'; j++)
        item->description[j] = delivery[j];

    int item_index = 0;
    while ((*items_buf)[item_index] != NULL)
        item_index += 1;

    (*items_buf)[item_index] = item;

    return true;
}

Receipt *MakeReceipt(char **data)
{
    printf("[LOG] - Entered MakeReceipt()\n");

    Item **items_buf[MAX_ITEMS];

    if(!ExtractSubstitutions(data, &items_buf))
    {
        FreeItems(items_buf);
        return NULL;
    }

    if(!ExtractRestOfItems(data, &items_buf)){
        FreeItems(items_buf);
        return NULL;
    }

    if(!AddDelivery(data, &items_buf))
    {
        FreeItems(items_buf);
        return NULL;
    }

    int items_buf_length = 0;
    while (items_buf[items_buf_length] != NULL)
        items_buf_length += 1;

    Item **items = (Item **)malloc((1 + items_buf_length) * sizeof(Item *));
    if (items == NULL)
    {
        FreeItems(items_buf);
        return NULL;
    }

    for (int i = 0; items_buf[i] != NULL; i++)
        items[i] = items_buf[i];

    Receipt *receipt;
    receipt->items = items;
    receipt->count = 0.0;
    for (int i = 0; receipt->items[i] != NULL; i++)
    {
        receipt->count += 1.0;
        receipt->total += receipt->items[i]->price;
    }

    printf("[LOG] - Leaving MakeReceipt()\n");
    return receipt;
}

Receipt *LoadReceipt(char *path)
{
    printf("[LOG] - Entered LoadReceipt()\n");

    char **data = ReadDataFromFile(path);
    if (data == NULL)
        return NULL;

    if (!CleanData(&data))
    {
        FreeData(data);
        return NULL;
    }

    Receipt *receipt = MakeReceipt(data);
    if (receipt == NULL)
    {
        FreeData(data);
        return NULL;
    }

    FreeData(data);
    return receipt;
}
