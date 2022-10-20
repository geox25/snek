main: main.cpp grid.hpp
	g++ main.cpp -o main -lncurses

build:
	g++ main.cpp -o main -lncurses
run: main.cpp grid.hpp
	./main
