#include "banking.h"
#include "file_utils.h"

void initializeFile(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        fp = fopen(filename, "wb");
        if (fp == NULL)
        {
            printf("\nError creating file: %s\n", filename);
            exit(1);
        }
    }
    fclose(fp);
}
