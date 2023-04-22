#pragma once

#include <iostream>
#include <cmath>

using namespace std;

struct point
{
    double x;
    double y;
};

struct figure
{
    string line;
    point* Points;
    int points_count;
    double radius;
    double square;
    double perimeter;
    string* Intersects;
    int intersects_count;
    int error;
};

void NumberCheck(string line, int begin, int end, int &error);
void WriteCheck(string line, int &open_bracket, int &close_bracket, int &error);
figure FigureCheck(string line, int begin, int end, int &error);
double TriangleSquare(point a, point b, point c);
bool SegmentIntersect(point a, point b, point c, point d);
void Intersect(figure *Figures, int correct_count);
figure* Lexer(string* Lines, int lines_count, int &correct_count);