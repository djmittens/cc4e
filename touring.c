#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main()
{
    const char *input[] = {
        "42 > 114 > 105 > 97 > ",
        "110 < < < < 66"
    };

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

    printf("sizeof(char):           %lld\n", sizeof(char));
    printf("sizeof(short):          %lld\n", sizeof(short));
    printf("sizeof(int):            %lld\n", sizeof(int));
    printf("sizeof(long):           %lld\n", sizeof(long));
    printf("sizeof(long long):      %lld\n", sizeof(long long));
    printf("sizeof(uint64_t):       %lld\n", sizeof(uint64_t));
    printf("sizeof(int64_t):        %lld\n", sizeof(int64_t));
    printf("sizeof(int8_t):         %lld\n", sizeof(int8_t));
    printf("sizeof(&input):         %lld\n", sizeof(&input));
    printf("sizeof(input):          %lld\n", sizeof(input));
}