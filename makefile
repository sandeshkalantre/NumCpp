all: numcpp
numcpp: main.o functions.o misc.o parser.o variables.o routines.o
	g++ main.o functions.o misc.o parser.o variables.o routines.o -lmpfr -lgmp -o numcpp
main.o: src/main.cpp
	g++ -c src/main.cpp
functions.o: src/functions.cpp src/functions.h src/parser.hpp
	g++ -c src/functions.cpp src/functions.h src/parser.hpp -lmpfr -lgmp
misc.o: src/misc.cpp src/misc.h
	g++ -c src/misc.cpp src/misc.h
parser.o: src/parser.cpp src/parser.hpp
	g++ -c src/parser.cpp src/parser.hpp -lmpfr -lgmp
variables.o: src/variables.cpp src/variables.h src/parser.hpp
	g++ -c src/variables.cpp src/variables.h src/parser.hpp -lmpfr -lgmp
routines.o: src/routines.cpp src/routines.h src/parser.hpp
	g++ -c src/routines.cpp src/routines.h src/parser.hpp -lmpfr -lgmp
clean:
	rm -rf *o numcpp

