all: numcpp
numcpp: main.o functions.o misc.o parser.o variables.o
	g++ main.o functions.o misc.o parser.o variables.o -o numcpp
main.o: src/main.cpp
	g++ -c src/main.cpp
functions.o: src/functions.cpp src/functions.h
	g++ -c src/functions.cpp src/functions.h
misc.o: src/misc.cpp src/misc.h
	g++ -c src/misc.cpp src/misc.h
parser.o: src/parser.cpp src/parser.hpp
	g++ -c src/parser.cpp src/parser.hpp
variables.o: src/variables.cpp src/variables.h src/parser.hpp
	g++ -c src/variables.cpp src/variables.h parser.hpp
clean:
	rm -rf *o numcpp

