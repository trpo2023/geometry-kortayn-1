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

    if(line.substr(0, open_bracket) != "circle" && line.substr(0, open_bracket) != "polygon")
    {
        cout << "\n" << line << "\n";
        cout << "^\nError at column 0: expected 'circle' or 'polygon'\n";
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

bool InsidePolygon(point vertex, figure polygon)
{
    int count = 0;

    for(int i = 0; i < polygon.points_count; i++)
    {
        point p1 = polygon.Points[i];
        point p2 = polygon.Points[(i + 1) % polygon.points_count];

        if(vertex.y > min(p1.y, p2.y)
        && vertex.y <= max(p1.y, p2.y)
        && vertex.x <= max(p1.x, p2.x)
        && p1.y != p2.y)
        {
            int xIntersection = (vertex.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
            if(p1.x == p2.x || vertex.x <= xIntersection) count++;
        }
    }

    return count % 2 == 1;
}

double Distance(point a, point b, point c)
{
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return fabs(dy * c.x - dx * c.y + b.x * a.y - b.y * a.x) / sqrt(pow(dy, 2) + pow(dx, 2));
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

            if(Figures[i].line.substr(0, 7) == "polygon" && Figures[j].line.substr(0, 7) == "polygon")
            {
                bool intersect = 0;

                for(int k = 0; k < Figures[i].points_count; k++)
                    if(InsidePolygon(Figures[i].Points[k], Figures[j])) intersect = 1;

                for(int k = 0; k < Figures[j].points_count; k++)
                    if(InsidePolygon(Figures[j].Points[k], Figures[i])) intersect = 1;

                if(intersect)
                    {
                        Figures[i].Intersects[Figures[i].intersects_count++] = to_string(j + 1) + ". polygon";
                        Figures[j].Intersects[Figures[j].intersects_count++] = to_string(i + 1) + ". polygon"; 
                    }
            }

            if(Figures[i].line.substr(0, 7) == "polygon" && Figures[j].line.substr(0, 6) == "circle") 
            {
                bool intersect = 0;

                for(int k = 0; k < Figures[i].points_count - 1; k++) 
                    if(Distance(Figures[i].Points[k], Figures[i].Points[k + 1], Figures[j].Points[0]) < Figures[j].radius)
                        intersect = 1;
                    
                if(Distance(Figures[i].Points[Figures[i].points_count - 1], Figures[i].Points[0], Figures[j].Points[0]) < Figures[j].radius
                || InsidePolygon(Figures[j].Points[0], Figures[i]))
                    intersect = 1;

                if(intersect)
                {
                    Figures[i].Intersects[Figures[i].intersects_count++] = to_string(j + 1) + ". circle";
                    Figures[j].Intersects[Figures[j].intersects_count++] = to_string(i + 1) + ". polygon"; 
                }
            }

            if(Figures[i].line.substr(0, 6) == "circle" && Figures[j].line.substr(0, 7) == "polygon")
            {
                bool intersect = 0;

                for(int k = 0; k < Figures[j].points_count - 1; k++)  
                    if(Distance(Figures[j].Points[k], Figures[j].Points[k + 1], Figures[i].Points[0]) < Figures[i].radius)
                        intersect = 1;
                    
                if(Distance(Figures[j].Points[Figures[i].points_count - 1], Figures[j].Points[0], Figures[i].Points[0]) < Figures[i].radius
                || InsidePolygon(Figures[i].Points[0], Figures[j]))
                    intersect = 1;

                if(intersect)
                {
                    Figures[i].Intersects[Figures[i].intersects_count++] = to_string(j + 1) + ". polygon";
                    Figures[j].Intersects[Figures[j].intersects_count++] = to_string(i + 1) + ". circle"; 
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