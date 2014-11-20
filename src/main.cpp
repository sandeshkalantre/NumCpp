#include <iostream>
#include <cstdio>

#include "parser.hpp"
using namespace std;



//definitions of the maps
std::map<std::string, Function> map_functions;
std::map<std::string, Number> map_variables;
std::map<std::string, Routine> map_routines;
std::map<std::string, ndArray> map_ndarrays;

//the global bools which are used when parsing fails
bool suppress_zero = false;
bool suppress_eval = false;

int main(int argc,char** argv)
{
    //default precision for the program
    mpfr_set_default_prec(DEFAULT_PRECISION);
    //command line arguments to the progran
    for(int i = 1; i < argc;i++)
    {
        if(argv[i][0] == '-' && argv[i][1] == 'p')
        {
            mpfr_set_default_prec(std::atoi(argv[i]));
            i+=2;
        }
    }

    //initialize the maps with standard data
    def_functions();
    def_variables();
    def_routines();
    def_ndarrays();

    //create a Parser object
    Parser parser;

    //the text to be declared at the start of the program
    std::cout<<"NumCpp"<<std::endl;
    std::cout<<

"\nNumCpp is a numerical library with an inbuilt parser for input\n\
that can be used to do numerical stuff such as:\n\
-integration\n\
-differentiation\n\
-matrix operations\n\
-root finding in an interval\n\
-FFT of an discrete array of complex numbers.\n\n\
Use help(routine_name) for help on a routine.\n\
Use exit() to exit the program.\n"

    <<std::endl;

    //the loop that runs the program
    while(true)
    {
        parser = Parser();
        string expr;
        string::iterator it_expr;

        //the prompt
        cout<<">>>";
        getline(cin,expr);

        //break if a single ; on a line is entered marking the end of the program
        //or if "exit()" is entered
        if(expr == ";" || expr.compare("exit()") == 0 || cin.eof())
        {
            cout<<"Bye.\n";
            return 0;
        }

        //to ensure that even if ; is forgotten at the end of input
        //the program still runs without an error
        expr += ";";

        it_expr = expr.begin();
        parser.parse(expr);
    }

    return 0;
}
