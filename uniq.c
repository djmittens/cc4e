#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{

    char line[1000];
    char keep[1000];

    FILE *handle = fopen("inputs/uniq.input.txt", "r");

    if (handle == NULL)
    {
        return EXIT_FAILURE;
    }

    while (fgets(line, 1000, handle) != NULL)
    {
        if (strcmp(line, keep) != 0)
        {
            strcpy(keep, line);
            printf("%s", keep);
        }
    }

    fclose(handle); // hah always close
                    // 
    return EXIT_SUCCESS;
}