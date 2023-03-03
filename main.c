#include <stdio.h>
#include <ctype.h>

int main()
{
    FILE* file;
    file = fopen("test.txt", "r");

    if (!file)
    {
        printf("Error: check file name\n");
        return 1;
    }

    int open_bracket = 0,
        close_bracket = 0,
        first_number = 0,
        second_number = 0,
        dot_count = 0,
        end_line = 0;

    int length = 0,
        element = 0, 
        error = 0;

    while(element != EOF)
    {
        element = fgetc(file);
        length++;
    }

    char a[length], b[6] = "circle";
    file = fopen("test.txt", "r");

    while(fgets(a, length - 1, file))
    {
        dot_count = 0;
        error = 0;
        printf("%s", a);

        for(int i = 0; i < length; i++)
            if(a[i] == '\n') end_line = i;

        for(int i = 0; i < end_line; i++)
        {
            if(a[i] == '(')
            {
                open_bracket = i;
                break;
            }
        }

        for(int i = 0; i < end_line; i++)
        {
            if(a[i] == ')')
            {
                close_bracket = i;
                break;
            }
        }

        for(int i = open_bracket + 1; i < end_line; i++)
        {
            if(a[i] == '(' && open_bracket != 0)
            {
                close_bracket = i;

                for(int j = 0; j < i; j++) printf(" ");
                printf("^\n");
                printf("Error at column %d: expected ')'\n", i);

                error = 1;;
                break;
            }
        }

        for(int i = open_bracket + 1; i < end_line; i++)
        {
            if(a[i] == ' ')
            {
                first_number = i;
                break;
            }
        }

        for(int i = first_number + 1; i < end_line; i++)
        {
            if(a[i] == ',')
            {
                second_number = i;
                break;
            }
        }

        for(int i = 0; i < open_bracket; i++)
        {
            if(a[i] != b[i])
            {
                printf("^\nError at column 0: expected 'circle'\n");
                error = 1;
                break;
            }
        }

        for(int i = open_bracket + 1; i < first_number; i++)
        {
            if(a[i] == '.') dot_count++;

            if((isdigit(a[i]) == 0 && a[i] != '.') || (dot_count > 1 && error == 0))
            {
                for(int j = 0; j < i; j++) printf(" ");

                printf("^\n");
                printf("Error at column %d: expected '<double>'\n", i);

                error = 1;
                break;
            }
        }

        for(int i = first_number + 1; i < second_number; i++)
        {
            if(a[i] == '.') dot_count++;

            if((isdigit(a[i]) == 0 && a[i] != '.') || (dot_count > 2 && error == 0))
            {
                for(int j = 0; j < i; j++) printf(" ");

                printf("^\n");
                printf("Error at column %d: expected '<double>'\n", i);

                error = 1;
                break;
            }
        }

        for(int i = second_number + 2; i < close_bracket; i++)
        {
            if(a[i] == '.') dot_count++;

            if((isdigit(a[i]) == 0 && a[i] != '.') || (dot_count > 3 && error == 0))
            {
                for(int j = 0; j < i; j++) printf(" ");

                printf("^\n");
                printf("Error at column %d: expected '<double>'\n", i);

                error = 1;
                break;
            }
        }
        
        if((a[close_bracket + 1] != '\n') && (a[close_bracket + 1] != EOF))
        {
            printf("\n");

            for(int j = 0; j < close_bracket + 1; j++) printf(" ");

            printf("^\n");
            printf("Error at column %d: unexpected tokens \n", close_bracket + 1);
            error = 1;
        }

        if(error == 0) printf("No Errors\n");

        printf("\n");
    }

    return 0;
}