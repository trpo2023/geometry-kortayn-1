#include <iostream>
#include <fstream>
#include <cmath>

#define pi 3.14159265358979323846

using namespace std;

int NumberCheck(string line, int begin, int end)
{
    int dot_count = 0,
        error = 0;

    for(int i = begin; i < end; i++)
    {
        if(line[i] == '.') dot_count++;

        if((isdigit(line[i]) == 0 && line[i] != '.') ||
            (line[begin] == '0' && isdigit(line[begin + 1]) != 0) ||
            dot_count > 1)
        {
            for(int j = 0; j < i; j++) cout << " ";

            cout << "^\nError at column " << i << ": expected '<double>'\n";
            error++;
            break;
        }
    }

    return error;
}

int WriteCheck(string line, int &open_bracket, int &close_bracket)
{
    int error = 0;

    for(int i = 0; i < int(line.length()); i++)
    {
        if(line[i] == '(' && open_bracket != 0)
        {
            close_bracket = i;

            for(int j = 0; j < i; j++) cout << " ";

            cout << "^\nError at column " << i << ": expected ')'\n";
            error++;
        }

        else if(line[i] == '(' && open_bracket == 0) open_bracket = i;

        if(line[i] == ')' && open_bracket == 0)
        {
            open_bracket = i;

            for(int j = 0; j < i; j++) cout << " ";

            cout << "^\nError at column " << i << ": expected '('\n";
            error++;
        }

        else if(line[i] == ')' && close_bracket == 0) close_bracket = i;
    }

    if(line.substr(0, open_bracket) != "circle" &&
        line.substr(0, open_bracket) != "triangle" &&
        line.substr(0, open_bracket) != "polygon")
    {
        cout << "^\nError at column 0: expected 'circle', 'triangle' or 'polygon'\n";
        error++;
    }


    if(close_bracket + 1 != int(line.length()))
    {
        for(int j = 0; j < close_bracket + 1; j++) cout << " ";

        cout << "^\nError at column " << close_bracket + 1 << ": unexpected tokens\n";
        error++;
    }

    return error;
}

void CircleCheck(string line, int open_bracket, int close_bracket, int error)
{
    int first_number = 0,
        second_number = 0;

    for(int i = open_bracket + 1; i < int(line.length()); i++)
    {
        if(line[i] == ' ')
        {
            first_number = i;
            break;
        }
    }

    for(int i = first_number + 1; i < int(line.length()); i++)
    {
        if(line[i] == ',')
        {
            second_number = i;
            break;
        }
    }

    error += NumberCheck(line, open_bracket + 1, first_number);
    error += NumberCheck(line, first_number + 1, second_number);
    error += NumberCheck(line, second_number + 2, close_bracket);

    if(!error)
    {
        float radius = stod(line.substr(second_number + 2, close_bracket));
        cout << "\nSquare = " << pi * pow(radius, 2)
             << "\nPerimeter = " << pi * 2 * radius << "\n";
    }
}

void TriangleCheck(string line, int open_bracket, int close_bracket, int error)
{
    cout << "\nFuture\n";
}

void PolygonCheck(string line, int open_bracket, int close_bracket, int error)
{
    cout << "\nFuture\n";
}

int main()
{
    ifstream file("test.txt");
    string line;

    while(getline(file, line))
    {
        int open_bracket = 0,
            close_bracket = 0,
            error = 0;

        cout << "\n" << line << "\n";

        error = WriteCheck(line, open_bracket, close_bracket);

        if(line.substr(0, open_bracket) == "circle")
            CircleCheck(line, open_bracket, close_bracket, error);

        if(line.substr(0, open_bracket) == "triangle")
            TriangleCheck(line, open_bracket, close_bracket, error);

        if(line.substr(0, open_bracket) == "polygon")
            PolygonCheck(line, open_bracket, close_bracket, error);
    }

    return 0;
}