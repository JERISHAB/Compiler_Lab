#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

void generateThreeAddressCode(char *lhs, char *expression)
{
    int tempCount = 1, opTop = -1, operTop = -1;
    char opStack[MAX], operStack[MAX][5], temp[5];
    char buffer[10]; // To hold multi-character operands

    for (int i = 0; expression[i]; i++)
    {
        if (isspace(expression[i]))
            continue;

        if (isalnum(expression[i]))
        {
            // Parse multi-character operands (identifiers or constants)
            int j = 0;
            while (isalnum(expression[i]) || expression[i] == '_')
            {
                buffer[j++] = expression[i++];
            }
            buffer[j] = '\0';
            strcpy(operStack[++operTop], buffer);
            i--; // Adjust index because the outer loop also increments it
        }
        else if (expression[i] == '(')
        {
            opStack[++opTop] = expression[i];
        }
        else if (expression[i] == ')')
        {
            while (opTop >= 0 && opStack[opTop] != '(')
            {
                sprintf(temp, "t%d", tempCount++);
                printf("%s = %s %c %s\n", temp, operStack[operTop - 1], opStack[opTop--], operStack[operTop]);
                strcpy(operStack[--operTop], temp);
            }
            opTop--; // Pop '('
        }
        else
        {
            // Operator
            while (opTop >= 0 && strchr("*/", opStack[opTop]))
            {
                sprintf(temp, "t%d", tempCount++);
                printf("%s = %s %c %s\n", temp, operStack[operTop - 1], opStack[opTop--], operStack[operTop]);
                strcpy(operStack[--operTop], temp);
            }
            opStack[++opTop] = expression[i];
        }
    }

    while (opTop >= 0)
    {
        sprintf(temp, "t%d", tempCount++);
        printf("%s = %s %c %s\n", temp, operStack[operTop - 1], opStack[opTop--], operStack[operTop]);
        strcpy(operStack[--operTop], temp);
    }

    // Assign the final result to the LHS variable
    printf("%s = %s\n", lhs, operStack[operTop]);
}

int main()
{
    char expression[MAX];
    char lhs[5], rhs[MAX];

    printf("Enter the expression (e.g., id1 = id2 + id3 + 60):\n");
    fgets(expression, sizeof(expression), stdin);

    // Extract LHS and RHS
    sscanf(expression, "%4[^=]=%[^\n]", lhs, rhs);

    printf("Expression: %s\nThree-address code:\n", expression);
    generateThreeAddressCode(lhs, rhs);

    return 0;
}