#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <Models.h>
#include <StringFunctions.h>
#include <HelperFunctions.h>

#define MAX_EMAIL_LINES 2048
#define MAX_LINE_BYTES 512
#define MAX_ITEMS 512

void FreeData(char **data)
{
    if (data == NULL)
        return;

    for (int i = 0; data[i] != NULL; i++)
    {
        free(data[i]);
    }
    free(data);

    return;
}

void FreeItems(Item ***items)
{
    if (*items == NULL)
        return;

    for (int i = 0; (*items)[i] != NULL; i++)
    {
        free((*items)[i]->description);
        free((*items)[i]);
    }
    free(*items);
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

bool CleanData(char ***data)
{
    printf("[LOG] - Entered CleanData()\n");

    int data_length = 0;
    while ((*data)[data_length] != NULL)
        data_length += 1;

    if (data_length == 0)
        return false;
    
    for (int i = 0; i < data_length; i++)
        CleanString(&(*data)[i]);

    int num_valid_strings = 0;
    for (int i = 0; (*data)[i] != NULL; i++)
        if (StringLength((*data)[i]) != 0)
            num_valid_strings += 1;

    char **clean_data = (char **)malloc((1 + num_valid_strings) * sizeof(char *));
    if (clean_data == NULL)
        return false;

    int clean_data_idx = 0;
    for (int i = 0; (*data)[i] != NULL; i++)
    {
        int string_length = StringLength((*data)[i]);
        if (string_length > 0)
        {
            clean_data[clean_data_idx] = (char *)malloc((1 + string_length) * sizeof(char));
            if (clean_data[clean_data_idx] == NULL)
                return false;
            
            for (int j = 0; j < string_length; j++)
                clean_data[clean_data_idx][j] = (*data)[i][j];

            clean_data_idx += 1;
        }
    }

    // move the receipt_data pointer to the clean data memory,
    // then free all the old receipt_data memory
    char **temp = (*data);

    (*data) = clean_data;

    FreeData(temp);

    printf("[LOG] - Leaving CleanData()\n");
    return true;
}

char **ReadDataFromFile(char *path)
{
    printf("[LOG] - Entered ReadDataFromFile()\n");

    FILE *f = fopen(path, "r");
    if (f == NULL)
        return NULL;

    char **data = (char **)malloc(MAX_EMAIL_LINES * sizeof(char *));
    if (data == NULL)
        return NULL;

    char line_buf[MAX_LINE_BYTES];
    for (int i = 0; fgets(line_buf, MAX_EMAIL_LINES, f); i++)
    {
        if (i >= MAX_EMAIL_LINES)
        {
            FreeData(data);
            return NULL;
        }
        int line_buf_length = StringLength(line_buf);

        data[i] = (char *)malloc((1 + line_buf_length) * sizeof(char));
        if (data[i] == NULL)
        {
            FreeData(data);
            return NULL;
        }

        for (int j = 0; j < line_buf_length; j++)
            data[i][j] = line_buf[j];

    }
    fclose(f);

    printf("[LOG] - Leaving ReadDataFromFile()\n");
    return data;
}

bool ExtractSubstitutions(char **data, Item ***items)
{
    int subs_index = GetIndex("Substitutions", data);
    if (subs_index == -1)
        return false;

    int roi_index = GetIndex("The rest of your items", data);
    if (roi_index == -1)
        return false;

    int item_index = 0;
    while ((*items)[item_index] != NULL)
        item_index += 1;

    bool begin_processing = false;
    for (int i = subs_index; i < roi_index; i++)
    {
        if (StringEquals(data[i], "Total"))
        {
            begin_processing = true;
            continue;
        }

        if (begin_processing)
        {

            Item *item = (Item *)malloc(sizeof(Item));
            if (item == NULL)
                return false;

            item->price = ParseFloat(data[i + 4]);
            item->quantity = (double)ParseInt(data[i + 5]);

            item->description = (char *)malloc((1 + StringLength(data[i + 6])) * sizeof(char));
            if (item->description == NULL)
            {
                free(item);
                return false;
            }

            for (int j = 0; data[i + 6][j] != '\0'; j++)
                item->description[j] = data[i + 6][j];

            (*items)[item_index] = item;
            item_index += 1;
            i += 7;
        }
    }

    return true;
}

bool ExtractRestOfItems(char **data, Item ***items_buf)
{
    int roi_index = GetIndex("The rest of your items", data);
    if (roi_index == -1)
        return false;

    int vat_index = GetIndex("Items marked with an  include VAT at 20%. Items marked with  include VAT at 5%.", data);
    if (vat_index == -1)
        return false;

    int item_index = 0;
    while ((*items_buf)[item_index] != NULL)
        item_index += 1;

    bool begin_processing = false;
    for (int i = roi_index; i < vat_index; i++)
    {
        if (StringEquals(data[i], "Total"))
        {
            begin_processing = true;
            continue;
        }

        if (begin_processing)
        {

            if (data[i][0] < 48 || data[i][0] > 57)
                continue;

            Item *item = (Item *)malloc(sizeof(Item));
            if (item == NULL)
                return false;

            item->price = ParseFloat(data[i + 3]);
            item->quantity = (double)ParseInt(data[i]);

            item->description = (char *)malloc((1 + StringLength(data[i + 1])) * sizeof(char));
            if (item->description == NULL)
            {
                free(item);
                return false;
            }

            for (int j = 0; data[i + 1][j] != '\0'; j++)
                item->description[j] = data[i + 1][j];

            (*items_buf)[item_index] = item;
            item_index += 1;
            i += 4;
        }
    }

    return true;
}

bool AddDelivery(char **data, Item ***items)
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
    while ((*items)[item_index] != NULL)
        item_index += 1;

    (*items)[item_index] = item;

    return true;
}

Receipt *MakeReceipt(char **data)
{
    printf("[LOG] - Entered MakeReceipt()\n");

    Item **items = (Item **)malloc(MAX_ITEMS * sizeof(Item *));
    if (items == NULL)
        return NULL;
    
    if(!ExtractSubstitutions(data, &items))
    {
        FreeItems(&items);
        return NULL;
    }

    int items_length = 0;
    while (items[items_length] != NULL)
    {
        printf("items[%i]->description = %s\n", items_length, items[items_length]->description);
        items_length += 1;
    }

    if(!ExtractRestOfItems(data, &items)){
        FreeItems(&items);
        return NULL;
    }

    items_length = 0;
    while (items[items_length] != NULL)
    {
        printf("items[%i]->description = %s\n", items_length, items[items_length]->description);
        items_length += 1;
    }

    if(!AddDelivery(data, &items))
    {
        FreeItems(&items);
        return NULL;
    }

    items_length = 0;
    while (items[items_length] != NULL)
    {
        printf("items[%i]->description = %s\n", items_length, items[items_length]->description);
        items_length += 1;
    }

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

    char **data = ReadDataFromFile(path); // Here
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
