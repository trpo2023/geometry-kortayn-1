#include "parser.hpp"
#include "lexer.hpp"

int main()
{
    int lines_count = 0, correct_count = 0;

    string* Lines = Parser(lines_count);
    figure* Figures = Lexer(Lines, lines_count, correct_count);

    for(int i = 0; i < correct_count; i++)
    {
        cout << "\n" << i + 1 << ". " << Figures[i].line << "\n";
        cout << "Square = " << Figures[i].square << "\n"
             << "Perimeter = " << Figures[i].perimeter << "\n";
             
        if(Figures[i].line.substr(0, 6) == "circle")
        {
            cout << "Intersects:\n";
            for(int k = 0; k < Figures[i].intersects_count; k++)
                cout << "\t" << Figures[i].intersects[k] << "\n";
        }

        delete Figures[i].Points;
    }

    delete Figures;
    return 0;
}