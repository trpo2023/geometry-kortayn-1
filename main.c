#include <stdio.h>
#include <ctype.h>

int NumberCheck(int begin, int end, int dot_count, int dot, char *a, int error)
{
    dot_count = 0;

    for(int i = begin; i < end; i++)
    {
        if(a[i] == '.')
        {
            dot_count++;
            dot = i;
        }

        if((isdigit(a[i]) == 0 && a[i] != '.') || dot_count > 1)
        {
            for(int j = 0; j < i; j++) printf(" ");

            printf("^\n");
            printf("Error at column %d: expected '<double>'\n", i);

            error = 1;
            break;
        }
    }

    return error;
}

int DigitChar(char digit)
{
	switch (digit)
	{
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		default : return -1;
	}
}

int main()
{
    FILE* file;
    file = fopen("test.txt", "r");

    if (!file)
    {
        printf("Error: check file name\n");
        return 1;
    }

    int i, j,
        open_bracket = 0,
        close_bracket = 0,
        first_number = 0,
        second_number = 0,
        dot_count = 0,
        dot = 0,
        end_line = 0,
        length = 0,
        element = 0, 
        error = 0;
        
    float radius = 0;

    while(element != EOF)
    {
        element = fgetc(file);
        length++;
    }

    char a[length], b[6] = "circle";
    file = fopen("test.txt", "r");

    while(fgets(a, length - 1, file))
    {
        error = 0;
        printf("\n%s", a);

        for(i = 0; i < length; i++)
            if(a[i] == '\n') end_line = i;

        for(i = 0; i < end_line; i++)
        {
            if(a[i] == '(')
            {
                open_bracket = i;
                break;
            }
        }

        for(i = open_bracket + 1; i < end_line; i++)
        {
            if(a[i] == ' ')
            {
                first_number = i;
                break;
            }
        }

        for(i = first_number + 1; i < end_line; i++)
        {
            if(a[i] == ',')
            {
                second_number = i;
                break;
            }
        }

        for(i = 0; i < end_line; i++)
        {
            if(a[i] == ')')
            {
                close_bracket = i;
                break;
            }
        }

        for(i = open_bracket + 1; i < end_line; i++)
        {
            if(a[i] == '(' && open_bracket != 0)
            {
                close_bracket = i;

                for(j = 0; j < i; j++) printf(" ");
                printf("^\n");
                printf("Error at column %d: expected ')'\n", i);

                error = 1;;
                break;
            }
        }

        for(i = 0; i < open_bracket; i++)
        {
            if(a[i] != b[i])
            {
                printf("^\nError at column 0: expected 'circle'\n");
                error = 1;
                break;
            }
        }

        error = NumberCheck(open_bracket + 1, first_number, dot_count, dot, a, error);
        error = NumberCheck(first_number + 1, second_number, dot_count, dot, a, error);
        error = NumberCheck(second_number + 2, close_bracket, dot_count, dot, a, error);
        
        if((a[close_bracket + 1] != '\n') && (a[close_bracket + 1] != EOF))
        {
            for(j = 0; j < close_bracket + 1; j++) printf(" ");

            printf("^\n");
            printf("Error at column %d: unexpected tokens\n", close_bracket + 1);
            error = 1;
        }

        if(error == 0)
        {
            for(i = dot - 1, j = 1; i > second_number + 1; i--, j++)
                radius += DigitChar(a[i]) * 10 * j;

            for(i = dot + 1, j = 1; i < close_bracket; i++, j++)
            {
                radius *= 10 * j;
                radius += DigitChar(a[i]);
            }

            radius /= 10 * j;
            printf("\nNo Errors\n");
        }
    }

    return 0;
}