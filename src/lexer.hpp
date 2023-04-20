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
    string* intersects;
    int intersects_count;
    int error;
};

int NumberCheck(string line, int begin, int end);
int WriteCheck(string line, int &open_bracket, int &close_bracket);
figure FigureCheck(string line, int begin, int end, int error);
figure* Lexer(string* Lines, int lines_count, int &correct_count);