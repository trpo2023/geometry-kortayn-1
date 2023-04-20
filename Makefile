CC = g++
flags = -Wall -Wextra -Werror

geometry: obj/geometry.o obj/parser.o obj/lexer.o
	$(CC) $(flags) -o bin/geometry.exe obj/geometry.o obj/parser.o obj/lexer.o
	./bin/geometry.exe

obj/geometry.o: src/geometry.cpp
	$(CC) -c $(flags) -o obj/geometry.o src/geometry.cpp
	
obj/parser.o: src/parser.cpp
	$(CC) -c $(flags) -o obj/parser.o src/parser.cpp

obj/lexer.o: src/lexer.cpp
	$(CC) -c $(flags) -o obj/lexer.o src/lexer.cpp