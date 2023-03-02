#include <stdio.h>

int main()
{
    FILE* file;
    file = fopen("test.txt", "r");

    if (!file)
    {
        printf("Error: check file name\n");
        return 1;
    }

    int length = 0, element = 0;

    while(1)
    {
        element = fgetc(file);
        if (element == EOF) break;
        length++;
    }

    printf("%d", length);

    return 0;
}