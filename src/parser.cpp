#include "parser.hpp"

void Parser(vector<string> &Lines)
{
    string line;
    ifstream file("geometry.txt");
    while(getline(file, line)) Lines.push_back(line);
}