#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Models.h>

bool IsStinkyChar(char c)
{
    if (c < 32 || c > 127) 
        return true;

    return false;
}

bool CleanLine(char *dirty_line)
{

    if (dirty_line == NULL)
        return false;

    int clean_line_idx = 0;
    for (int i = 0; dirty_line[i] != '\0'; i++)
    {
        if (!IsStinkyChar(dirty_line[i])){
            dirty_line[clean_line_idx] = dirty_line[i];
            clean_line_idx++;
        }
    }

    dirty_line[clean_line_idx] = '\0';

    return true;
}

bool StringEquals(char *str1, char *str2)
{

    if (str1 == NULL || str2 == NULL)
        return NULL;

    if (strlen(str1) != strlen(str2))
        return false;

    for (int i = 0; i < strlen(str1); i++)
    {
        if (str1[i] != str2[i])
            return false;
    }

    return true;
}

bool CleanData(char **clean_data, char **email_content, int *num_clean_lines)
{
    const unsigned EMAIL_LINES = *num_clean_lines;

    int clean_line_idx = 0;
    for (int i = 0; i < EMAIL_LINES; i++)
    {
        // clean line in place
        if(!CleanLine(email_content[i]))
            return false;

        if (StringEquals(email_content[i], "Payment details"))
            break;

        // if the cleaned line has some content, add to clean data
        if (strlen(email_content[i]) > 0)
        {
            clean_data[clean_line_idx] = (char *)malloc((strlen(email_content[i]) + 1) * sizeof(char));
            strcpy(clean_data[clean_line_idx], email_content[i]);
            clean_line_idx += 1;
        }
    }

    *num_clean_lines = clean_line_idx;
    return true;
}

bool ReadData(char **email_content, const char *path, const unsigned MAX_EMAIL_LINES)
{
    const unsigned MAX_LINE_BYTES = 5000;

    // open the file and check for error
    FILE *f = fopen(path, "r");
    if (f == NULL)
        false;

    // read each line from the file, storing it in email_content.
    // if malloc fails or MAX_NUM_LINES is reached then exit
    int num_lines_read = 0;
    char buffer[MAX_LINE_BYTES];
    while (fgets(buffer, MAX_LINE_BYTES, f))
    {
        // allocate enough memory to store the next line (+1 for null terminator)
        int line_length = strlen(buffer) + 1;
        email_content[num_lines_read] = (char *)malloc(line_length * sizeof(char));
        if (email_content[num_lines_read] == NULL)
            return false;

        // copy str from buffer to email_content
        strcpy(email_content[num_lines_read], buffer);

        // increment num_email_lines and check if we've gone over the line limit
        num_lines_read += 1;
        if (num_lines_read >= MAX_EMAIL_LINES)
            return false;
    }

    fclose(f);
    return true;
}

Receipt *MakeReceipt(char **clean_data)
{
    return NULL;
}

Receipt *LoadReceipt(const char *path)
{
    // read raw data in from the email
    const unsigned MAX_EMAIL_LINES = 10000;

    char **email_content = (char **)malloc(MAX_EMAIL_LINES * sizeof(char *));
    if (email_content == NULL)
        exit(EXIT_FAILURE);

    // email content will be filled with data
    // num_email_lines will be updated with actual number of email lines
    if (!ReadData(email_content, path, MAX_EMAIL_LINES))
    {
        for (int i = 0; i < MAX_EMAIL_LINES; i++)
            free(email_content[i]);
        free(email_content);
        exit(EXIT_FAILURE);
    }

    // clean the raw data
    int num_email_lines = -1;
    for (int i = 0; email_content[i] != NULL; i++)
        num_email_lines = i;

    int num_clean_lines = num_email_lines;

    char **clean_data = (char **)malloc(num_clean_lines * sizeof(char *));
    if (clean_data == NULL)
    {
        for (int i = 0; i < MAX_EMAIL_LINES; i++)
                free(email_content[i]);
        free(email_content);
        exit(EXIT_FAILURE);
    }

    // clean_data will be filled with clean lines
    // num_clean_lines will be updated with the actual number of clean lines
    if(!CleanData(clean_data, email_content, &num_clean_lines))
    {
        for (int i = 0; i < MAX_EMAIL_LINES; i++)
            free(email_content[i]);
        for (int i = 0; i < num_email_lines; i++)
            free(clean_data[i]);

        free(email_content);
        free(clean_data);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_clean_lines; i++)
        printf("%s\n", clean_data[i]);

    // process cleaned data into receipt object
    // TODO: Need to work out how memory allocation will work here?
    Receipt *receipt = MakeReceipt(clean_data);

    // free clean_data as no longer needed
    for (int i = 0; i < num_clean_lines; i++)
        free(clean_data[i]);
    free(clean_data);

    // return pointer to receipt object
    return receipt;
}
