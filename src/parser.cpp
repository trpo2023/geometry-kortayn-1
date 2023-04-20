#include "parser.hpp"

string* Parser(int &lines_count)
{
    ifstream file("geometry.txt");
    string line;
    while(getline(file, line)) lines_count++;

    file.clear();
    file.seekg(0);

    string* Lines = new string[lines_count];
    for(int i = 0; i < lines_count; i++) getline(file, Lines[i]);

    return Lines;
}