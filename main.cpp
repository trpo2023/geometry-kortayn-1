#include <iostream>
#include <cmath>

#define pi 3.14159265358979323846

using namespace std;

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

double NumberString(int begin, int end, char *string)
{
    int i, j,
        dot = 0;

    double number = 0;

    for(i = begin; i < end; i++)
        if(string[i] == '.') dot = i;

    for(i = begin, j = dot - begin - 1; i < dot; i++, j--)
        number += DigitChar(string[i]) * pow(10, j);

    number *= pow(10, end - dot - 1);

    for(i = dot + 1, j = end - dot - 2; i < end; i++, j--)
        number += DigitChar(string[i]) * pow(10, j);

    number /= pow(10, end - dot - 1);

    return number;
}

int NumberCheck(int begin, int end, int error, char *string)
{
    int i, j,
        dot_count = 0;

    for(i = begin; i < end; i++)
    {
        if(string[i] == '.') dot_count++;

        if((isdigit(string[i]) == 0 && string[i] != '.') || dot_count > 1)
        {
            for(j = 0; j < i; j++) cout << " ";

            cout << "^\nError at column " << i << ": expected '<double>'\n";
            error = 1;
            break;
        }
    }

    return error;
}

int main()
{
    FILE* file;
    file = fopen("test.txt", "r");

    if (!file)
    {
        cout << "Error: check file name\n";
        return 1;
    }

    int i, j,
        open_bracket,
        close_bracket,
        first_number,
        second_number,
        end_line,
        element, 
        error,
        length = 0;

    double radius;

    while(element != EOF)
    {
        element = fgetc(file);
        length++;
    }

    char string[length], figure[] = "circle";

    rewind(file);

    while(fgets(string, length - 1, file))
    {
        open_bracket = 0,
        close_bracket = 0,
        first_number = 0,
        second_number = 0,
        end_line = 0,
        error = 0;
        
        cout << "\n" << string;

        for(i = 0; i < length; i++)
            if(string[i] == '\n') end_line = i;

        for(i = 0; i < end_line; i++)
        {
            if(string[i] == '(')
            {
                open_bracket = i;
                break;
            }

            if(string[i] == ')' && open_bracket == 0)
            {
                open_bracket = i;

                for(j = 0; j < i; j++) cout << " ";

                cout << "^\nError at column " << i << ": expected '('\n";
                error = 1;
                break;
            }
        }

        for(i = open_bracket + 1; i < end_line; i++)
        {
            if(string[i] == ' ')
            {
                first_number = i;
                break;
            }
        }

        for(i = first_number + 1; i < end_line; i++)
        {
            if(string[i] == ',')
            {
                second_number = i;
                break;
            }
        }

        for(i = second_number + 1; i < end_line; i++)
        {
            if(string[i] == ')')
            {
                close_bracket = i;
                break;
            }

            if(string[i] == '(' && open_bracket != 0)
            {
                close_bracket = i;

                for(j = 0; j < i; j++) cout << " ";

                cout << "^\nError at column " << i << ": expected ')'\n";
                error = 1;
                break;
            }
        }

        for(i = 0; i < open_bracket; i++)
        {
            if(string[i] != figure[i])
            {
                cout << "^\nError at column 0: expected 'circle'\n";
                error = 1;
                break;
            }
        }

        error = NumberCheck(open_bracket + 1, first_number, error, string);
        error = NumberCheck(first_number + 1, second_number, error, string);
        error = NumberCheck(second_number + 2, close_bracket, error, string);
        
        if(string[close_bracket + 1] != '\n')
        {
            for(j = 0; j < close_bracket + 1; j++) cout << " ";

            cout << "^\nError at column " << close_bracket + 1 << ": unexpected tokens\n";
            error = 1;
        }

        if(error == 0)
        {
            radius = NumberString(second_number + 2, close_bracket, string);
            cout
            << "\nSquare = " << pi * pow(radius, 2)
            << "\nPerimeter = " << pi * 2 * radius;
        }
    }

    fclose(file);

    return 0;
}