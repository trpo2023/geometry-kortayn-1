CC = g++
flags = -Wall -Wextra -Werror

main: obj/main.o obj/parser.o obj/lexer.o
	$(CC) $(flags) -o bin/main.exe obj/main.o obj/parser.o obj/lexer.o
	./bin/main.exe

obj/main.o: src/main.cpp
	$(CC) -c $(flags) -o obj/main.o src/main.cpp
	
obj/parser.o: src/parser.cpp
	$(CC) -c $(flags) -o obj/parser.o src/parser.cpp

obj/lexer.o: src/lexer.cpp
	$(CC) -c $(flags) -o obj/lexer.o src/lexer.cpp
