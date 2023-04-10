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
    int number;
    string line;
    point *Points;
    int points_count;
    double square;
    double perimeter;
    int error;
};

int NumberCheck(string line, int begin, int end, int flag)
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
            if(!flag)
            {
                cout << "\n" << line << "\n";
                for(int j = 0; j < i; j++) cout << " ";
                cout << "^\nError at column " << i << ": expected '<double>'\n";
            }

            error++;
            break;
        }
    }

    return error;
}

int WriteCheck(string line, int &open_bracket, int &close_bracket, int flag)
{
    int error = 0;

    for(int i = 0; i < int(line.length()); i++)
    {
        if(line[i] == '(' && open_bracket != 0)
        {
            close_bracket = i;

            if(!flag)
            {
                cout << "\n" << line << "\n";
                for(int j = 0; j < i; j++) cout << " ";
                cout << "^\nError at column " << i << ": expected ')'\n";
            }

            error++;
        }

        else if(line[i] == '(' && open_bracket == 0) open_bracket = i;

        if(line[i] == ')' && open_bracket == 0)
        {
            open_bracket = i;

            if(!flag)
            {
                cout << "\n" << line << "\n";
                for(int j = 0; j < i; j++) cout << " ";
                cout << "^\nError at column " << i << ": expected '('\n";
            }

            error++;
        }

        else if(line[i] == ')' && close_bracket == 0) close_bracket = i;
    }

    if(line.substr(0, open_bracket) != "circle" &&
        line.substr(0, open_bracket) != "triangle" &&
        line.substr(0, open_bracket) != "polygon")
    {
        if(!flag)
        {
            cout << "\n" << line << "\n";
            cout << "^\nError at column 0: expected 'circle', 'triangle' or 'polygon'\n";
        }

        error++;
    }


    if(close_bracket + 1 != int(line.length()))
    {
        if(!flag)
        {
            cout << "\n" << line << "\n";
            for(int j = 0; j < close_bracket + 1; j++) cout << " ";
            cout << "^\nError at column " << close_bracket + 1 << ": unexpected tokens\n";
        }

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

figure FigureCheck(string line, int *Commas, int comma_count, int begin, int end, int error, int flag)
{
    int space;

    point *Points = new point[comma_count + 1];
    figure correct_figure;

    for(int i = 0; i < comma_count; i++)
    {
        for(int j = begin + 1; j < Commas[i]; j++)
            if(line[j] == ' ') space = j;

        error += NumberCheck(line, begin + 1, space, flag);
        error += NumberCheck(line, space + 1, Commas[i], flag);

        if(!error)
        {
            Points[i].x = stod(line.substr(begin + 1, space));
            Points[i].y = stod(line.substr(space + 1, Commas[i]));
        }

        begin = Commas[i] + 1;
    }

    if(line.substr(0, 6) == "circle")
    {
        error += NumberCheck(line, Commas[0] + 2, end, flag);

        if(!error)
        {
            float radius = stod(line.substr(Commas[0] + 2, end));

            delete Commas;

            correct_figure.line = line;
            correct_figure.Points = Points;
            correct_figure.points_count = comma_count;
            correct_figure.square = pi * pow(radius, 2);
            correct_figure.perimeter = pi * 2 * radius;
            correct_figure.error = 0;

            delete Points;

            return correct_figure;
        }

        else
        {
            delete Commas;
            delete Points;

            figure Error;
            Error.error = 1;
            return Error;
        }
    }

    for(int i = Commas[comma_count - 1] + 2; i < end; i++)
        if(line[i] == ' ') space = i;

    error += NumberCheck(line, Commas[comma_count - 1] + 2, space, flag);
    error += NumberCheck(line, space + 1, end, flag);

    if(line.substr(0, 8) == "triangle")
    {
        if(!error)
        {
            Points[comma_count].x = stod(line.substr(Commas[comma_count - 1] + 2, space));
            Points[comma_count].y = stod(line.substr(space + 1, end));

            double a = sqrt(pow((Points[1].x - Points[0].x), 2) + pow((Points[1].y - Points[0].y), 2));
            double b = sqrt(pow((Points[2].x - Points[1].x), 2) + pow((Points[2].y - Points[1].y), 2));
            double c = sqrt(pow((Points[0].x - Points[2].x), 2) + pow((Points[0].y - Points[2].y), 2));

            double perimeter = a + b + c;

            delete Commas;

            correct_figure.line = line;
            correct_figure.Points = Points;
            correct_figure.points_count = comma_count + 1;
            correct_figure.square = sqrt(perimeter / 2 * (perimeter / 2 - a) * (perimeter / 2 - b) * (perimeter / 2 - c));
            correct_figure.perimeter = perimeter;
            correct_figure.error = 0;

            delete Points;

            return correct_figure;
        }

        else
        {
            delete Commas;
            delete Points;

            figure Error;
            Error.error = 1;
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

            delete Commas;

            correct_figure.line = line;
            correct_figure.Points = Points;
            correct_figure.points_count = comma_count + 1;
            correct_figure.square = fabs(square / 2);
            correct_figure.perimeter = perimeter;
            correct_figure.error = 0;

            delete Points;

            return correct_figure;
        }

        else
        {
            delete Commas;
            delete Points;

            figure Error;
            Error.error = 1;
            return Error;
        }
    }

    delete Commas;
    delete Points;

    figure Error;
    Error.error = 1;
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
    int correct = 0, flag = 0;

    for(int i = 0; i < lines_count; i++)
    {
        int open_bracket = 0,
            close_bracket = 0,
            comma_count = 0,
            error = 0;

        error = WriteCheck(Lines[i], open_bracket, close_bracket, flag);
        int *Commas = CommaCount(Lines[i], comma_count, open_bracket, close_bracket);
        figure input_figure = FigureCheck(Lines[i], Commas, comma_count, open_bracket, close_bracket, error, flag);

        if(!input_figure.error) correct_count++;
    }

    flag = 1;

    figure *Figures = new figure[correct_count];

    for(int i = 0; i < lines_count; i++)
    {
        int open_bracket = 0,
            close_bracket = 0,
            comma_count = 0,
            error = 0;

        error = WriteCheck(Lines[i], open_bracket, close_bracket, flag);
        int *Commas = CommaCount(Lines[i], comma_count, open_bracket, close_bracket);
        figure input_figure = FigureCheck(Lines[i], Commas, comma_count, open_bracket, close_bracket, error, flag);

        if(!input_figure.error)
        {
            input_figure.number = correct + 1;
            Figures[correct] = input_figure;
            correct++;
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
        cout << "\n" << Figures[i].number << ". " << Figures[i].line << "\n"
             << "Square = " << Figures[i].square << "\n"
             << "Perimeter = " << Figures[i].perimeter << "\n";
    }

    delete Figures;

    return 0;
}