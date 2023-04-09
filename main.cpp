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

void CircleCheck(string line, int *commas, int begin, int end, int error)
{
    int space;

    for(int i = begin + 1; i < commas[0]; i++)
        if(line[i] == ' ') space = i;

    error += NumberCheck(line, begin + 1, space);
    error += NumberCheck(line, space + 1, commas[0]);
    error += NumberCheck(line, commas[0] + 2, end);

    if(!error)
    {
        float radius = stod(line.substr(commas[0] + 2, end));
        cout << "\nSquare = " << pi * pow(radius, 2)
             << "\nPerimeter = " << pi * 2 * radius << "\n";
    }

    delete commas;
}

void TriangleCheck(string line, int *commas, int comma_count, int begin, int end, int error)
{
    int space;

    double **xy = new double*[comma_count + 1];
    for(int i = 0; i < comma_count + 1; i++) xy[i] = new double[2];

    for(int i = 0; i < comma_count; i++)
    {
        for(int j = begin + 1; j < commas[i]; j++)
            if(line[j] == ' ') space = j;

        error += NumberCheck(line, begin + 1, space);
        error += NumberCheck(line, space + 1, commas[i]);

        if(!error)
        {
            xy[i][0] = stod(line.substr(begin + 1, space));
            xy[i][1] = stod(line.substr(space + 1, commas[i]));
        }

        begin = commas[i] + 1;
    }

    for(int i = commas[comma_count - 1] + 2; i < end; i++)
        if(line[i] == ' ') space = i;

    error += NumberCheck(line, commas[comma_count - 1] + 2, space);
    error += NumberCheck(line, space + 1, end);

    if(!error)
    {
        xy[comma_count][0] = stod(line.substr(commas[comma_count - 1] + 2, space));
        xy[comma_count][1] = stod(line.substr(space + 1, end));

        double a = sqrt(pow((xy[1][0] - xy[0][0]), 2) + pow((xy[1][1] - xy[0][1]), 2));
        double b = sqrt(pow((xy[2][0] - xy[1][0]), 2) + pow((xy[2][1] - xy[1][1]), 2));
        double c = sqrt(pow((xy[0][0] - xy[2][0]), 2) + pow((xy[0][1] - xy[2][1]), 2));

        double p = a + b + c;

        cout << "\nSquare = " << sqrt(p / 2 * (p / 2 - a) * (p / 2 - b) * (p / 2 - c))
             << "\nPerimeter = " << p << "\n";
    }

    delete commas;
}

void PolygonCheck(string line, int *commas, int comma_count, int begin, int end, int error)
{
    int space;

    double **xy = new double*[comma_count + 1];
    for(int i = 0; i < comma_count + 1; i++) xy[i] = new double[2];

    for(int i = 0; i < comma_count; i++)
    {
        for(int j = begin + 1; j < commas[i]; j++)
            if(line[j] == ' ') space = j;

        error += NumberCheck(line, begin + 1, space);
        error += NumberCheck(line, space + 1, commas[i]);

        if(!error)
        {
            xy[i][0] = stod(line.substr(begin + 1, space));
            xy[i][1] = stod(line.substr(space + 1, commas[i]));
        }

        begin = commas[i] + 1;
    }

    for(int i = commas[comma_count - 1] + 2; i < end; i++)
        if(line[i] == ' ') space = i;

    error += NumberCheck(line, commas[comma_count - 1] + 2, space);
    error += NumberCheck(line, space + 1, end);

    if(!error)
    {
        xy[comma_count][0] = stod(line.substr(commas[comma_count - 1] + 2, space));
        xy[comma_count][1] = stod(line.substr(space + 1, end));

        double perimeter = 0, square = 0;

        for (int i = 0; i < comma_count + 1; i++)
        {
            int j = (i + 1) % (comma_count + 1);

            double dx = xy[i][0] - xy[j][0];
            double dy = xy[i][1] - xy[j][1];

            perimeter += sqrt(pow(dx, 2) + pow(dy, 2));
            square += xy[i][0] * xy[j][1] - xy[j][0] * xy[i][1];
        }

        cout << "\nSquare = " << fabs(square / 2)
             << "\nPerimeter = " << perimeter << "\n";
    }

    delete commas;
}

string *Parser(int &lines_count)
{
    ifstream file("test.txt");
    string line;

    while(getline(file, line)) lines_count++;

    file.clear();
    file.seekg(0);

    string *lines = new string[lines_count];

    for(int i = 0; i < lines_count; i++) getline(file, lines[i]);

    return lines;
}

void Lexer(int lines_count, string *lines)
{
    for(int i = 0; i < lines_count; i++)
    {
        int open_bracket = 0,
            close_bracket = 0,
            comma_count = 0,
            error = 0;

        cout << "\n" << lines[i] << "\n";

        error = WriteCheck(lines[i], open_bracket, close_bracket);

        int *commas = CommaCount(lines[i], comma_count, open_bracket, close_bracket);

        if(lines[i].substr(0, open_bracket) == "circle")
            CircleCheck(lines[i], commas, open_bracket, close_bracket, error);

        if(lines[i].substr(0, open_bracket) == "triangle")
            TriangleCheck(lines[i], commas, comma_count, open_bracket, close_bracket, error);

        if(lines[i].substr(0, open_bracket) == "polygon")
            PolygonCheck(lines[i], commas, comma_count, open_bracket, close_bracket, error);
    }

    delete lines;
}

int main()
{
    int lines_count = 0;

    string *lines = Parser(lines_count);
    Lexer(lines_count, lines);
    
    return 0;
}