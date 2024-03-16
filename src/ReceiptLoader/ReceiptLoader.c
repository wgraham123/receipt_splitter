#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

#define EXIT_MEMORY_ALLOCATION_FAILURE 2
#define EXIT_FILE_ERROR 3

bool Streq(char* str1, char* str2)
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

long GetFileSize(const char *path)
{
    struct stat file_status;

    if (stat(path, &file_status) < 0)
        exit(EXIT_FILE_ERROR);

    return file_status.st_size;
}

bool IsStinkyChar(char c)
{
    if (c < 32 || c > 127)
        return true;

    return false;
}

char* CleanLine(const char *dirty_line)
{

    if (dirty_line == NULL)
        return "";

    int line_length = strlen(dirty_line) + 1;
    char *clean_line = (char *)malloc(line_length * sizeof(char));
    if (clean_line == NULL)
        exit(EXIT_MEMORY_ALLOCATION_FAILURE);
    
    int clean_line_idx = 0;
    for (int i = 0; i < line_length; i++)
    {
        if (IsStinkyChar(dirty_line[i]))
            continue;

        clean_line[clean_line_idx] = dirty_line[i];
        clean_line_idx++;
    }

    return clean_line;
}

char** ReadReceipt(const char* path)
{
    const unsigned MAX_NUM_LINES = 10000;
    const unsigned MAX_LINE_BYTES = 5000;

    long file_size = GetFileSize(path);

    FILE *f = fopen(path, "r");
    if (f == NULL)
        exit(EXIT_FILE_ERROR);

    char **content = (char **) malloc(MAX_NUM_LINES * sizeof(char *));
    if (content == NULL)
        exit(EXIT_MEMORY_ALLOCATION_FAILURE);

    char buffer[MAX_LINE_BYTES];
    int num_lines_read = 0;
    while (fgets(buffer, MAX_LINE_BYTES, f)) 
    {
        int line_length = strlen(buffer) + 1; // +1 for null terminator
        if (line_length == 1)
            continue;

        char *clean_line = CleanLine(buffer);
        line_length = strlen(clean_line) + 1; // +1 for null terminator
        if (line_length == 1)
            continue;

        if (Streq(clean_line, "Payment details") != 0)
            break;

        content[num_lines_read] = (char *)malloc(line_length * sizeof(char));
        if (content[num_lines_read] == NULL)
            exit(EXIT_MEMORY_ALLOCATION_FAILURE);

        strcpy(content[num_lines_read], clean_line);
        free(clean_line);

        num_lines_read++;
        if (num_lines_read >= MAX_NUM_LINES)
            exit(EXIT_FILE_ERROR);
    }

    fclose(f);

    for (int i = num_lines_read; i < MAX_NUM_LINES; i++)
        free(content[i]);
    
    return content;
}
