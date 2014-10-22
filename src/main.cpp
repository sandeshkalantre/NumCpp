#include <iostream>
#include "parser.hpp"
#include "functions.h"
#include "routines.h"
using namespace std;

//definitions of the maps
std::map<std::string, Function> map_functions;
std::map<std::string, double> map_variables;
std::map<std::string, Routine> map_routines;

int main()
{
    //initialize the maps with standard data
    def_functions();
    def_variables();
    def_routines();

    //create a Parser object
    Parser parser;

    //the loop that runs the program
    while(true)
    {
        parser = Parser();
        string expr;
        string::iterator it_expr;

        //the prompt
        cout<<">";
        getline(cin,expr);

        //break if a single ; on a line is entered marking the end of the program
        //or if "exit()" is entered
        if(expr == ";" || expr.compare("exit()") == 0)
        {
            return 0;
        }

        //to ensure that even if ; is forgotten at the end of input
        //the program still runs
        expr += ";";

        it_expr = expr.begin();
        parser.parse(expr);
    }

    return 0;
}

