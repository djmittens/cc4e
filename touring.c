#include <stdio.h>
#include <string.h>

int main()
{
    char *input[] = {
        "42 > 114 > 105 > 97 > ",
        "110 < < < < 66"};

    char memory[256], token[256];
    int head = 0, value;

    memory[0] = '\0';

    for (int i = 0; i < sizeof(input) / sizeof(input[0]); ++i)
    {
        printf("%s\n", input[i]);
        int len = strlen(input[i]);
        printf("Len: %d\n", len);
        char *buf = (char *)malloc(len + 1);
        strcpy(buf, input[i]);
        char *pch;
        pch = strtok(buf, " ");
        while (pch != NULL)
        {
            printf("%s\n", pch);
            switch (*pch)
            {
            case '<':
                head--;
                break;
            case '>':
                head++;
                break;
            default:
                memory[head] = (char)atoi(pch);
                break;
            }
            pch = strtok(NULL, " ");
        }
        free(buf);
    }

    printf("Memory:\n%s\n", memory);
}