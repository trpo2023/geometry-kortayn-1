#include "parser.hpp"
#include "lexer.hpp"

int main()
{
    vector<string> Lines;
    vector<figure> Figures;
    Parser(Lines);
    Lexer(Lines, Figures);

    for(figure i : Figures)
    {
        int count = 1;
        cout << "\n" << count++ << ". " << i.line << "\n"
             << "Square = " << i.square << "\n"
             << "Perimeter = " << i.perimeter << "\n"
             << "Intersects:\n";
        for(string j : i.Intersects)
            cout << "\t" << j << "\n";
    }

    return 0;
}