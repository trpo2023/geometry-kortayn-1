#include "parser.hpp"
#include "lexer.hpp"

int main()
{
    int lines_count = 0, correct_count = 0;

    string *Lines = Parser(lines_count);
    figure *Figures = Lexer(Lines, lines_count, correct_count);

    for(int i = 0; i < correct_count; i++)
    {
        cout << "\n" << i + 1 << ". " << Figures[i].line << "\n";
        if(Figures[i].line.substr(0, 6) == "circle")
        {
            for(int j = 0; j < Figures[i].points_count; j++)
            cout << "Center Point: (" << Figures[i].Points[j].x << ", " << Figures[i].Points[j].y << ")\n";
            cout << "Radius = " << Figures[i].radius << "\n";
        }
        else for(int j = 0; j < Figures[i].points_count; j++)
            cout << "Point " << j + 1 << ": (" << Figures[i].Points[j].x << ", " << Figures[i].Points[j].y << ")\n";
        cout << "Square = " << Figures[i].square << "\n"
             << "Perimeter = " << Figures[i].perimeter << "\n";

        delete Figures[i].Points;
    }

    delete Figures;
    return 0;
}