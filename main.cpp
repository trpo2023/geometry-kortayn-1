#include <iostream>
#include <fstream>
#include <cmath>
#define pi 3.14159265358979323846

using namespace std;

struct point
{
    double x;
    double y;
};

struct figure
{
    string line;
    point *Points;
    int points_count;
    double radius;
    double square;
    double perimeter;
    int error;
};

int NumberCheck(string line, int begin, int end)
{
    int dot_count = 0,
        error = 0;

    for(int i = begin; i < end; i++)
    {
        if(line[i] == '.') dot_count++;

        if(((isdigit(line[i]) == 0 && line[i] != '.') ||
            (line[begin] == '0' && isdigit(line[begin + 1]) != 0) ||
            dot_count > 1))
        {
            cout << "\n" << line << "\n";
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

            cout << "\n" << line << "\n";
            for(int j = 0; j < i; j++) cout << " ";
            cout << "^\nError at column " << i << ": expected ')'\n";
            error++;
        }

        else if(line[i] == '(' && open_bracket == 0) open_bracket = i;

        if(line[i] == ')' && open_bracket == 0)
        {
            open_bracket = i;

            cout << "\n" << line << "\n";
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
        cout << "\n" << line << "\n";
        cout << "^\nError at column 0: expected 'circle', 'triangle' or 'polygon'\n";
        error++;
    }

    if(close_bracket + 1 != int(line.length()))
    {
        cout << "\n" << line << "\n";
        for(int j = 0; j < close_bracket + 1; j++) cout << " ";
        cout << "^\nError at column " << close_bracket + 1 << ": unexpected tokens\n";
        error++;
    }

    return error;
}

int *CommaCount(string line, int &comma_count, int open_bracket, int close_bracket)
{
    for(int i = open_bracket + 1; i < close_bracket; i++)
        if(line[i] == ',') comma_count++;

    int *Commas = new int[comma_count];

    for(int i = open_bracket + 1, j = 0; i < close_bracket; i++)
        if(line[i] == ',') Commas[j++] = i;

    return Commas;
}

figure FigureCheck(string line, int *Commas, int comma_count, int begin, int end, int error)
{
    int space;

    point *Points = new point[comma_count + 1];
    figure correct_figure;

    for(int i = 0; i < comma_count; i++)
    {
        for(int j = begin + 1; j < Commas[i]; j++)
            if(line[j] == ' ') space = j;

        error += NumberCheck(line, begin + 1, space);
        error += NumberCheck(line, space + 1, Commas[i]);

        if(!error)
        {
            Points[i].x = stod(line.substr(begin + 1, space));
            Points[i].y = stod(line.substr(space + 1, Commas[i]));
        }

        begin = Commas[i] + 1;
    }

    if(line.substr(0, 6) == "circle")
    {
        error += NumberCheck(line, Commas[0] + 2, end);

        if(!error)
        {
            correct_figure.line = line;
            correct_figure.Points = Points;
            correct_figure.points_count = comma_count;
            correct_figure.radius = stod(line.substr(Commas[0] + 2, end));
            correct_figure.square = pi * pow(stod(line.substr(Commas[0] + 2, end)), 2);
            correct_figure.perimeter = pi * 2 * stod(line.substr(Commas[0] + 2, end));

            delete Commas;
            return correct_figure;
        }

        else
        {
            figure Error;
            Error.error = 1;
            delete Commas;
            return Error;
        }
    }

    for(int i = Commas[comma_count - 1] + 2; i < end; i++)
        if(line[i] == ' ') space = i;

    error += NumberCheck(line, Commas[comma_count - 1] + 2, space);
    error += NumberCheck(line, space + 1, end);

    if(line.substr(0, 8) == "triangle")
    {
        if(!error)
        {
            Points[comma_count].x = stod(line.substr(Commas[comma_count - 1] + 2, space));
            Points[comma_count].y = stod(line.substr(space + 1, end));

            double a = sqrt(pow((Points[1].x - Points[0].x), 2) + pow((Points[1].y - Points[0].y), 2));
            double b = sqrt(pow((Points[2].x - Points[1].x), 2) + pow((Points[2].y - Points[1].y), 2));
            double c = sqrt(pow((Points[0].x - Points[2].x), 2) + pow((Points[0].y - Points[2].y), 2));

            correct_figure.line = line;
            correct_figure.Points = Points;
            correct_figure.points_count = comma_count + 1;
            correct_figure.square = sqrt((a + b + c) / 2 * ((a + b + c) / 2 - a) * ((a + b + c) / 2 - b) * ((a + b + c) / 2 - c));
            correct_figure.perimeter = (a + b + c);

            delete Commas;
            return correct_figure;
        }

        else
        {
            figure Error;
            Error.error = 1;
            delete Commas;
            return Error;
        }
    }

    if(line.substr(0, 7) == "polygon")
    {
        if(!error)
        {
            double perimeter = 0, square = 0;

            Points[comma_count].x = stod(line.substr(Commas[comma_count - 1] + 2, space));
            Points[comma_count].y = stod(line.substr(space + 1, end));

            for (int i = 0; i < comma_count + 1; i++)
            {
                int j = (i + 1) % (comma_count + 1);
                double dx = Points[i].x - Points[j].x;
                double dy = Points[i].y - Points[j].y;
                perimeter += sqrt(pow(dx, 2) + pow(dy, 2));
                square += Points[i].x * Points[j].y - Points[j].x * Points[i].y;
            }

            correct_figure.line = line;
            correct_figure.Points = Points;
            correct_figure.points_count = comma_count + 1;
            correct_figure.square = fabs(square / 2);
            correct_figure.perimeter = perimeter;

            delete Commas;
            return correct_figure;
        }

        else
        {
            figure Error;
            Error.error = 1;
            delete Commas;
            return Error;
        }
    }

    figure Error;
    Error.error = 1;
    delete Commas;
    return Error;
}

string *Parser(int &lines_count)
{
    ifstream file("test.txt");
    string line;
    while(getline(file, line)) lines_count++;

    file.clear();
    file.seekg(0);

    string *Lines = new string[lines_count];
    for(int i = 0; i < lines_count; i++) getline(file, Lines[i]);

    return Lines;
}

figure *Lexer(string *Lines, int lines_count, int &correct_count)
{
    figure *Figures = new figure[lines_count];

    for(int i = 0; i < lines_count; i++)
    {
        int open_bracket = 0,
            close_bracket = 0,
            comma_count = 0,
            error = 0;

        error = WriteCheck(Lines[i], open_bracket, close_bracket);
        int *Commas = CommaCount(Lines[i], comma_count, open_bracket, close_bracket);
        figure input_figure = FigureCheck(Lines[i], Commas, comma_count, open_bracket, close_bracket, error);

        if(input_figure.error != 1)
        {
            Figures[correct_count] = input_figure;
            correct_count++;
        }
    }

    delete Lines;
    return Figures;
}

int main()
{
    int lines_count = 0, correct_count = 0;

    string *Lines = Parser(lines_count);
    figure *Figures = Lexer(Lines, lines_count, correct_count);

    for(int i = 0; i < correct_count; i++)
    {
        cout << "\n" << i + 1 << ". " << Figures[i].line << "\n";
        if(Figures[i].line.substr(0, 6) == "circle")
        {
            for(int j = 0; j < Figures[i].points_count; j++)
            cout << "Center Point: (" << Figures[i].Points[j].x << ", " << Figures[i].Points[j].y << ")\n";
            cout << "Radius = " << Figures[i].radius << "\n";
        }
        else for(int j = 0; j < Figures[i].points_count; j++)
            cout << "Point " << j + 1 << ": (" << Figures[i].Points[j].x << ", " << Figures[i].Points[j].y << ")\n";
        cout << "Square = " << Figures[i].square << "\n"
             << "Perimeter = " << Figures[i].perimeter << "\n";
    }

    delete Figures;
    return 0;
}