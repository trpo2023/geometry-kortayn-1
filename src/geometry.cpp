#include "parser.hpp"
#include "lexer.hpp"

int main()
{
    int lines_count = 0, correct_count = 0;

    string* Lines = Parser(lines_count);
    figure* Figures = Lexer(Lines, lines_count, correct_count);

    for(int i = 0; i < correct_count; i++)
    {
        cout << "\n" << i + 1 << ". " << Figures[i].line << "\n"
             << "Square = " << Figures[i].square << "\n"
             << "Perimeter = " << Figures[i].perimeter << "\n"
             << "Intersects:\n";
        for(int j = 0; j < Figures[i].intersects_count; j++)
            cout << "\t" << Figures[i].Intersects[j] << "\n";

        delete Figures[i].Points;
        delete Figures[i].Intersects;
    }

    delete Figures;
    return 0;
}