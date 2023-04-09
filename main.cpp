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

int *CommaCount(string line, int &comma_count, int open_bracket, int close_bracket)
{
    int *commas;

    for(int i = open_bracket + 1; i < close_bracket; i++)
        if(line[i] == ',') comma_count++;

    commas = new int[comma_count];

    for(int i = open_bracket + 1, j = 0; i < close_bracket; i++)
        if(line[i] == ',') commas[j++] = i;
    
    return commas;
}

void CircleCheck(string line, int *commas, int open_bracket, int close_bracket, int error)
{
    int space = 0;

    for(int i = open_bracket + 1; i < commas[0]; i++)
        if(line[i] == ' ') space = i;

    error += NumberCheck(line, open_bracket + 1, space);
    error += NumberCheck(line, space + 1, commas[0]);
    error += NumberCheck(line, commas[0] + 2, close_bracket);

    if(!error)
    {
        float radius = stod(line.substr(commas[0] + 2, close_bracket));
        cout << "\nSquare = " << pi * pow(radius, 2)
             << "\nPerimeter = " << pi * 2 * radius << "\n";
    }
}

void TriangleCheck(string line, int *commas, int comma_count, int open_bracket, int close_bracket, int error)
{
    int space;

    int **xy = new int*[comma_count];
    for(int i = 0; i < comma_count; i++) xy[i] = new int[2];

    for(int i = 0; i < comma_count; i++)
    {
        for(int j = open_bracket + 1; j < commas[i]; j++)
            if(line[j] == ' ') space = j;

        error += NumberCheck(line, open_bracket + 1, space);
        error += NumberCheck(line, space + 1, commas[i]);

        if(!error)
        {
            xy[i][1] = stod(line.substr(open_bracket + 1, space));
            xy[i][2] = stod(line.substr(space + 1, commas[i]));
            cout << xy[i][1] << " " << xy[i][2] << "\n";
        }

        open_bracket = commas[i] + 1;
    }

    for(int i = commas[comma_count - 1] + 2; i < close_bracket; i++)
        if(line[i] == ' ') space = i;

    error += NumberCheck(line, commas[comma_count - 1] + 2, space);
    error += NumberCheck(line, space + 1, close_bracket);

    if(!error)
    {
        xy[comma_count - 1][1] = stod(line.substr(commas[comma_count - 1] + 2, space));
        xy[comma_count - 1][2] = stod(line.substr(space + 1, close_bracket));
        cout << xy[comma_count - 1][1] << " " << xy[comma_count - 1][2] << "\n";
    }
}

void PolygonCheck(string line, int *commas, int comma_count, int open_bracket, int close_bracket, int error)
{
    int space;

    int **xy = new int*[comma_count];
    for(int i = 0; i < comma_count; i++) xy[i] = new int[2];

    for(int i = 0; i < comma_count; i++)
    {
        for(int j = open_bracket + 1; j < commas[i]; j++)
            if(line[j] == ' ') space = j;

        error += NumberCheck(line, open_bracket + 1, space);
        error += NumberCheck(line, space + 1, commas[i]);

        if(!error)
        {
            xy[i][1] = stod(line.substr(open_bracket + 1, space));
            xy[i][2] = stod(line.substr(space + 1, commas[i]));
            cout << xy[i][1] << " " << xy[i][2] << "\n";
        }

        open_bracket = commas[i] + 1;
    }

    for(int i = commas[comma_count - 1] + 2; i < close_bracket; i++)
        if(line[i] == ' ') space = i;

    error += NumberCheck(line, commas[comma_count - 1] + 2, space);
    error += NumberCheck(line, space + 1, close_bracket);

    if(!error)
    {
        xy[comma_count - 1][1] = stod(line.substr(commas[comma_count - 1] + 2, space));
        xy[comma_count - 1][2] = stod(line.substr(space + 1, close_bracket));
        cout << xy[comma_count - 1][1] << " " << xy[comma_count - 1][2] << "\n";
    }
}

int main()
{
    ifstream file("test.txt");
    string line;

    while(getline(file, line))
    {
        int open_bracket = 0,
            close_bracket = 0,
            comma_count = 0,
            error = 0;

        cout << "\n" << line << "\n";

        error = WriteCheck(line, open_bracket, close_bracket);

        int *commas = CommaCount(line, comma_count, open_bracket, close_bracket);

        if(line.substr(0, open_bracket) == "circle")
            CircleCheck(line, commas, open_bracket, close_bracket, error);

        if(line.substr(0, open_bracket) == "triangle")
            TriangleCheck(line, commas, comma_count, open_bracket, close_bracket, error);

        if(line.substr(0, open_bracket) == "polygon")
            PolygonCheck(line, commas, comma_count, open_bracket, close_bracket, error);
    }

    return 0;
}