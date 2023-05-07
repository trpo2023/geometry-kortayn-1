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
    int space;

    vector<int> Commas;

    for(int i = begin + 1; i < end; i++)
        if(line[i] == ',') Commas.push_back(i);

    vector<point> Points;

    for(int comma : Commas)
    {
        for(int j = begin + 1; j < comma; j++)
            if(line[j] == ' ') space = j;

        NumberCheck(line, begin + 1, space, error);
        NumberCheck(line, space + 1, comma, error);

        if(!error)
        {
            point vertex = {stod(line.substr(begin + 1, space)),
                            stod(line.substr(space + 1, comma))};
            Points.push_back(vertex);
        }

        begin = comma + 1;
    }

    figure correct_figure;

    if(line.substr(0, 6) == "circle")
    {
        NumberCheck(line, Commas[0] + 2, end, error);

        if(!error)
        {
            correct_figure.line = line;
            correct_figure.Points = Points;
            correct_figure.radius = stod(line.substr(Commas[0] + 2, end));
            correct_figure.square = M_PI * pow(stod(line.substr(Commas[0] + 2, end)), 2);
            correct_figure.perimeter = M_PI * 2 * stod(line.substr(Commas[0] + 2, end));
        }
    }

    else
    {
        for(int i = Commas[Commas.size() - 1] + 2; i < end; i++)
            if(line[i] == ' ') space = i;

        NumberCheck(line, Commas[Commas.size() - 1] + 2, space, error);
        NumberCheck(line, space + 1, end, error);
        
        if(!error)
        {
            double perimeter = 0, square = 0;

            point vertex = {stod(line.substr(Commas[Commas.size() - 1] + 2, space)),
                            stod(line.substr(space + 1, end))};
            Points.push_back(vertex);

            for (int i = 0; i < int(Commas.size() + 1); i++)
            {
                int j = (i + 1) % (Commas.size() + 1);
                double dx = Points[i].x - Points[j].x;
                double dy = Points[i].y - Points[j].y;
                perimeter += sqrt(pow(dx, 2) + pow(dy, 2));
                square += Points[i].x * Points[j].y - Points[j].x * Points[i].y;
            }

            correct_figure.line = line;
            correct_figure.Points = Points;
            correct_figure.square = fabs(square / 2);
            correct_figure.perimeter = perimeter;
        }
    }

    if(!error)
        return correct_figure;

    else
    {
        figure Error;
        Error.error = 1;
        return Error;
    }
}

bool InsidePolygon(point vertex, figure polygon)
{
    int count = 0;

    for(int i = 0; i < int(polygon.Points.size()); i++)
    {
        point p1 = polygon.Points[i];
        point p2 = polygon.Points[(i + 1) % polygon.Points.size()];

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
    return fabs(dy * c.x - dx * c.y + b.x * a.y - b.y * a.x)
           / sqrt(pow(dy, 2) + pow(dx, 2));
}

int TriangleSquare(point a, point b, point c) {
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}
 
bool OverlaySegments(int a, int b, int c, int d)
{
	if (a > b) swap (a, b);
	if (c > d) swap (c, d);
	return max(a,c) <= min(b,d);
}
 
bool SegmentsIntersect(point a, point b, point c, point d)
{
	return OverlaySegments(a.x, b.x, c.x, d.x)
		&& OverlaySegments(a.y, b.y, c.y, d.y)
		&& TriangleSquare(a, b, c) * TriangleSquare(a, b, d) <= 0
		&& TriangleSquare(c, d, a) * TriangleSquare(c, d, b) <= 0;
}

void Intersect(vector<figure> &Figures)
{
    for(int i = 0; i < int(Figures.size()); i++)
        for(int j = i + 1; j < int(Figures.size()); j++)
        {
            if(Figures[i].line.substr(0, 6) == "circle" && Figures[j].line.substr(0, 6) == "circle")
            {
                double distance = sqrt(pow(Figures[i].Points[0].x - Figures[j].Points[0].x, 2)
                                     + pow(Figures[i].Points[0].y - Figures[j].Points[0].y, 2));

                if(Figures[i].radius + Figures[j].radius > distance)
                {
                    Figures[i].Intersects.push_back(to_string(j + 1) + ". circle");
                    Figures[j].Intersects.push_back(to_string(i + 1) + ". circle");
                }
            }

            if(Figures[i].line.substr(0, 7) == "polygon" && Figures[j].line.substr(0, 7) == "polygon")
            {
                bool intersect = 0;

                for(int k = 0; k < int(Figures[i].Points.size()); k++)
                    if(InsidePolygon(Figures[i].Points[k], Figures[j]))
                        intersect = 1;

                for(int k = 0; k < int(Figures[j].Points.size()); k++)
                    if(InsidePolygon(Figures[j].Points[k], Figures[i]))
                        intersect = 1;

                for(int k = 0; k < int(Figures[i].Points.size() - 1); k++)
                    for(int l = 0; l < int(Figures[j].Points.size() - 1); l++)
                        if(SegmentsIntersect(Figures[i].Points[k],
                                             Figures[i].Points[k + 1],
                                             Figures[j].Points[l],
                                             Figures[j].Points[l + 1]))
                            intersect = 1;

                if(SegmentsIntersect(Figures[i].Points[Figures[i].Points.size() - 1],
                                     Figures[i].Points[0],
                                     Figures[j].Points[Figures[j].Points.size() - 1],
                                     Figures[j].Points[0]))
                    intersect = 1;

                if(intersect)
                {
                    Figures[i].Intersects.push_back(to_string(j + 1) + ". polygon");
                    Figures[j].Intersects.push_back(to_string(i + 1) + ". polygon"); 
                }
            }

            if(Figures[i].line.substr(0, 7) == "polygon" && Figures[j].line.substr(0, 6) == "circle") 
            {
                bool intersect = 0;

                for(int k = 0; k < int(Figures[i].Points.size() - 1); k++)
                    if(Distance(Figures[i].Points[k], Figures[i].Points[k + 1],
                                Figures[j].Points[0]) < Figures[j].radius)
                        intersect = 1;
                    
                if(Distance(Figures[i].Points[Figures[i].Points.size() - 1],
                            Figures[i].Points[0], Figures[j].Points[0]) < Figures[j].radius
                            || InsidePolygon(Figures[j].Points[0], Figures[i]))
                    intersect = 1;

                if(intersect)
                {
                    Figures[i].Intersects.push_back(to_string(j + 1) + ". circle");
                    Figures[j].Intersects.push_back(to_string(i + 1) + ". polygon"); 
                }
            }

            if(Figures[i].line.substr(0, 6) == "circle" && Figures[j].line.substr(0, 7) == "polygon")
            {
                bool intersect = 0;

                for(int k = 0; k < int(Figures[j].Points.size() - 1); k++)  
                    if(Distance(Figures[j].Points[k], Figures[j].Points[k + 1],
                                Figures[i].Points[0]) < Figures[i].radius)
                        intersect = 1;
                    
                if(Distance(Figures[j].Points[Figures[i].Points.size() - 1], Figures[j].Points[0],
                            Figures[i].Points[0]) < Figures[i].radius
                            || InsidePolygon(Figures[i].Points[0], Figures[j]))
                    intersect = 1;

                if(intersect)
                {
                    Figures[i].Intersects.push_back(to_string(j + 1) + ". polygon");
                    Figures[j].Intersects.push_back(to_string(i + 1) + ". circle"); 
                }
            }
        }
}

void Lexer(vector<string> &Lines, vector<figure> &Figures)
{
    for(string line : Lines)
    {
        int open_bracket = 0,
            close_bracket = 0,
            error = 0;

        WriteCheck(line, open_bracket, close_bracket, error);
        figure input_figure = FigureCheck(line, open_bracket, close_bracket, error);

        if(input_figure.error != 1)
            Figures.push_back(input_figure);
    }

    Intersect(Figures);
}