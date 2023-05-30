#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reverse(char *t)
{
    int i, j, len;
    char tmp;
    len = strlen(t);
    for (i = 0, j = len - 1;; i++, j--)
    {
        if (j < i)
            break;
        tmp = t[i];
        t[i] = t[j];
        t[j] = tmp;
    }
    return;
}

void itob(int n, char *s)
{
    char res[33];

    int end = 0;
    for (int i = 0; i < 32; ++i)
    {
        if ((n & (1 << i)) > 0)
        {
            res[i] = '1';
            end = i;
        }
        else
        {
            res[i] = '0';
        }
    }
    res[end + 1] = '\0'; // null terminate this guy
    reverse(res);
    strcpy(s, res);
}

void itoh(int n, char *s)
{
    const char lookup[] = "0123456789ABCDEF";
    const int ndigit = sizeof(int);          // number of digits in an int
    const int dmask = 0xf;                   // mask for a single digit hex

    char res[ndigit + 1];
    int end = 0;

    for (int i = 0; i < ndigit; ++i)
    {
        int digit = (n >> (4 * i)) & dmask;
        if(digit > 0)
        {
            end = i;
        }

        res[i] = lookup[digit];
    }

    res[end + 1] = '\0'; // null terminate this guy
    reverse(res);
    strcpy(s, res);
}

int main()
{
    char s[1000];

    itob(42, s);
    printf("42 in base-2 is %s\n", s);
    itoh(42, s);
    printf("42 in base-16 is %s\n", s);

    itob(16, s);
    printf("16 in base-2 is %s\n", s);
    itoh(16, s);
    printf("16 in base-16 is %s\n", s);

    itob(59, s);
    printf("59 in base-2 is %s\n", s);
    itoh(59, s);
    printf("59 in base-16 is %s\n", s);

    itob(100, s);
    printf("100 in base-2 is %s\n", s);
    itoh(100, s);
    printf("100 in base-16 is %s\n", s);

    itob(254, s);
    printf("254 in base-2 is %s\n", s);
    itoh(254, s);
    printf("254 in base-16 is %s\n", s);
}