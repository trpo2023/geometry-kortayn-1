#pragma once

#include <iostream>
#include <vector>
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
    vector<point> Points;
    double radius;
    double square;
    double perimeter;
    vector<string> Intersects;
    int error;
};

void NumberCheck(string line, int begin, int end, int &error);
void WriteCheck(string line, int &open_bracket, int &close_bracket, int &error);
figure FigureCheck(string line, int begin, int end, int &error);
bool InsidePolygon(point vertex, figure polygon);
double Distance(point a, point b, point c);
int TriangleSquare(point a, point b, point c);
bool OverlaySegments(int a, int b, int c, int d);
bool SegmentsIntersect(point a, point b, point c, point d);
void Intersect(vector<figure> &Figures);
void Lexer(vector<string> &Lines, vector<figure> &Figures);