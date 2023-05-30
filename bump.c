#include <stdio.h>
int main()
{
    int bump();
    printf("bump() returns %d\n", bump());
    printf("bump() returns %d\n", bump());
    printf("bump() returns %d\n", bump());
    printf("bump() returns %d\n", bump());
    printf("bump() returns %d\n", bump());
}

int bump()
{
    static int var = 0;
    return var++;
}