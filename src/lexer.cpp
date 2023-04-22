#include "lexer.hpp"

void NumberCheck(string line, int begin, int end, int &error)
{
    int dot_count = 0,
        minus_count = 0;

    for(int i = begin; i < end; i++)
    {
        if(line[i] == '.') dot_count++;
        if(line[i] == '-') minus_count++;

        if((isdigit(line[i]) == 0 && line[i] != '.' && line[i] != '-')
            || (line[begin] == '0' && isdigit(line[begin + 1]) != 0)
            || dot_count > 1 || minus_count > 1)
        {
            cout << "\n" << line << "\n";
            for(int j = 0; j < i; j++) cout << " ";
            cout << "^\nError at column " << i << ": expected '<double>'\n";
            error++;
            break;
        }
    }
}

void WriteCheck(string line, int &open_bracket, int &close_bracket, int &error)
{
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

    if(line.substr(0, open_bracket) != "circle"
        && line.substr(0, open_bracket) != "triangle"
        && line.substr(0, open_bracket) != "polygon")
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
}

figure FigureCheck(string line, int begin, int end, int &error)
{
    int space,
        comma_count = 0;

    for(int i = begin + 1; i < end; i++)
        if(line[i] == ',') comma_count++;

    int* Commas = new int[comma_count];

    for(int i = begin + 1, j = 0; i < end; i++)
        if(line[i] == ',') Commas[j++] = i;

    point* Points = new point[comma_count + 1];

    for(int i = 0; i < comma_count; i++)
    {
        for(int j = begin + 1; j < Commas[i]; j++)
            if(line[j] == ' ') space = j;

        NumberCheck(line, begin + 1, space, error);
        NumberCheck(line, space + 1, Commas[i], error);

        if(!error)
        {
            Points[i].x = stod(line.substr(begin + 1, space));
            Points[i].y = stod(line.substr(space + 1, Commas[i]));
        }

        begin = Commas[i] + 1;
    }

    figure correct_figure;

    if(line.substr(0, 6) == "circle")
    {
        NumberCheck(line, Commas[0] + 2, end, error);

        if(!error)
        {
            correct_figure.line = line;
            correct_figure.Points = Points;
            correct_figure.points_count = comma_count;
            correct_figure.radius = stod(line.substr(Commas[0] + 2, end));
            correct_figure.square = M_PI * pow(stod(line.substr(Commas[0] + 2, end)), 2);
            correct_figure.perimeter = M_PI * 2 * stod(line.substr(Commas[0] + 2, end));
            correct_figure.intersects_count = 0;
        }
    }

    else
    {
        for(int i = Commas[comma_count - 1] + 2; i < end; i++)
            if(line[i] == ' ') space = i;

        NumberCheck(line, Commas[comma_count - 1] + 2, space, error);
        NumberCheck(line, space + 1, end, error);
        
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
            correct_figure.intersects_count = 0;
        }
    }

    if(!error)
    {
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

double TriangleSquare(point a, point b, point c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}
 
bool SegmentIntersect(point a, point b, point c, point d)
{
	return TriangleSquare(a, b, c) * TriangleSquare(a, b, d) <= 0
		&& TriangleSquare(c, d, a) * TriangleSquare(c, d, b) <= 0;
}

double Distance(figure* Figures, int i, int j, int k)
{
    return fabs((Figures[i].Points[k + 1].y - Figures[i].Points[k].y) * Figures[j].Points[0].x
              - (Figures[i].Points[k + 1].x - Figures[i].Points[k].x) * Figures[j].Points[0].y
              + Figures[i].Points[k + 1].x * Figures[i].Points[k].y
              - Figures[i].Points[k + 1].y * Figures[i].Points[k].x)
              / sqrt(pow((Figures[i].Points[k + 1].y - Figures[i].Points[k].y), 2)
              + pow((Figures[i].Points[k + 1].x - Figures[i].Points[k].x), 2));
}

void Intersect(figure* Figures, int correct_count)
{
    for(int i = 0; i < correct_count; i++)
        for(int j = i + 1; j < correct_count; j++)
        {
            if(Figures[i].line.substr(0, 6) == "circle" && Figures[j].line.substr(0, 6) == "circle")
            {
                double distance = sqrt(pow(Figures[i].Points[0].x - Figures[j].Points[0].x, 2)
                                     + pow(Figures[i].Points[0].y - Figures[j].Points[0].y, 2));

                if(Figures[i].radius + Figures[j].radius > distance)
                {
                    Figures[i].Intersects[Figures[i].intersects_count++] = to_string(j + 1) + ". circle";
                    Figures[j].Intersects[Figures[j].intersects_count++] = to_string(i + 1) + ". circle";
                }
            }

            else if(Figures[i].line.substr(0, 7) == "polygon" && Figures[j].line.substr(0, 7) == "polygon")
            {
                bool intersect = 0;

                for(int k = 0; k < Figures[i].points_count - 1; k++)
                    for(int l = 0; l < Figures[j].points_count - 1; l++)
                        if(SegmentIntersect(Figures[i].Points[k],
                                            Figures[i].Points[k + 1],
                                            Figures[j].Points[l],
                                            Figures[j].Points[l + 1])) intersect = 1;

                if(SegmentIntersect(Figures[i].Points[Figures[i].points_count - 1],
                                    Figures[i].Points[0],
                                    Figures[j].Points[Figures[j].points_count - 1],
                                    Figures[j].Points[0])) intersect = 1;
                        
                if(intersect)
                    {
                        Figures[i].Intersects[Figures[i].intersects_count++] = to_string(j + 1) + ". polygon";
                        Figures[j].Intersects[Figures[j].intersects_count++] = to_string(i + 1) + ". polygon"; 
                    }
            }

            else
            {
                if(Figures[i].line.substr(0, 7) == "polygon") 
                {
                    bool intersect = 0;

                    for(int k = 0; k < Figures[i].points_count - 1; k++) 
                        if(Distance(Figures, i, j, k) < Figures[j].radius) intersect = 1;
                    
                    if(Distance(Figures, i, j, 0) < Figures[j].radius) intersect = 1;

                    if(intersect)
                    {
                        Figures[i].Intersects[Figures[i].intersects_count++] = to_string(j + 1) + ". circle";
                        Figures[j].Intersects[Figures[j].intersects_count++] = to_string(i + 1) + ". polygon"; 
                    }
                }

                else
                {
                    bool intersect = 0;

                    for(int k = 0; k < Figures[j].points_count - 1; k++)  
                        if(Distance(Figures, j, i, k) < Figures[i].radius) intersect = 1;

                    if(Distance(Figures, j, i, 0) < Figures[i].radius) intersect = 1;

                    if(intersect)
                    {
                        Figures[i].Intersects[Figures[i].intersects_count++] = to_string(j + 1) + ". polygon";
                        Figures[j].Intersects[Figures[j].intersects_count++] = to_string(i + 1) + ". circle"; 
                    }
                }
            }
        }
}

figure* Lexer(string* Lines, int lines_count, int &correct_count)
{
    figure* Figures = new figure[lines_count];

    for(int i = 0; i < lines_count; i++)
    {
        int open_bracket = 0,
            close_bracket = 0,
            error = 0;

        WriteCheck(Lines[i], open_bracket, close_bracket, error);
        figure input_figure = FigureCheck(Lines[i], open_bracket, close_bracket, error);

        if(input_figure.error != 1)
            Figures[correct_count++] = input_figure;
    }

    for(int i = 0; i < correct_count; i++) Figures[i].Intersects = new string[correct_count];

    Intersect(Figures, correct_count);

    delete Lines;
    return Figures;
}