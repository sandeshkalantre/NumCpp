#include <iostream>
#include "parser.hpp"
#include "functions.h"
#include "routines.h"
using namespace std;

std::map<std::string, Function> map_functions;
std::map<std::string, double> map_variables;
std::map<std::string, Routine> map_routines;

int main()
{
    def_functions();
    def_variables();
    def_routines();
    Parser parser;


    while(true)
    {
        parser = Parser();
        string expr;
        string::iterator it_expr;
        cout<<">";
        getline(cin,expr);
        expr += ";";
        //it_expr++;
        //*(it_expr) = ';';
        it_expr = expr.begin();
        if(expr == ";"){return 0;}
        parser.parse(expr);
        expr.clear();
    }





    return 0;
}

