all: NumCpp
NumCpp: main.o functions.o misc.o parser.o variables.o routines.o complex.o sim_eqn.o
	g++ main.o functions.o misc.o parser.o variables.o routines.o complex.o sim_eqn.o -lmpfr -lgmp -o NumCpp
main.o: src/main.cpp
	g++ -c src/main.cpp
functions.o: src/functions.cpp src/functions.h src/parser.hpp
	g++ -c src/functions.cpp src/functions.h src/parser.hpp -lmpfr -lgmp
misc.o: src/misc.cpp src/misc.h src/parser.hpp
	g++ -c src/misc.cpp src/misc.h src/parser.hpp
parser.o: src/parser.cpp src/parser.hpp
	g++ -c src/parser.cpp src/complex.cpp src/sim_eqn.cpp src/parser.hpp -lmpfr -lgmp
variables.o: src/variables.cpp src/variables.h src/parser.hpp
	g++ -c src/variables.cpp src/variables.h src/parser.hpp -lmpfr -lgmp
routines.o: src/routines.cpp src/routines.h src/parser.hpp src/errors.h
	g++ -c src/routines.cpp src/routines.h src/parser.hpp src/errors.h -lmpfr -lgmp
complex.o: src/complex.cpp src/parser.hpp -lmpfr -lgmp
	g++ -c src/complex.cpp src/parser.hpp -lmpfr -lgmp
sim_eqn.o: src/sim_eqn.cpp src/parser.hpp -lmpfr -lgmp
	g++ -c src/sim_eqn.cpp src/parser.hpp -lmpfr -lgmp

clean:
	rm -rf *.o NumCpp

