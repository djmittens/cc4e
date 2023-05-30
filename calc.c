#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char line[256];
    char opcode;
    float value, display = 0.0;

    FILE *handle = fopen("inputs/calc.input.txt", "r");

    while (fgets(line, 256, handle) != NULL)
    {
        // Use sscanf to parse data from a string
        sscanf(line, "%c %f", &opcode, &value);

        /* Replace this with your code */
        switch (opcode)
        {
        case '=':
            display = value;
            break;
        case '*':
            display *= value;
            break;
        case '+':
            display += value;
            break;
        case '-':
            display -= value;
            break;
        case '/':
            display /= value;
            break;
        case 'S':
            return EXIT_SUCCESS;
        }

        printf("Display: %.2f\n", display);
    }

}
 