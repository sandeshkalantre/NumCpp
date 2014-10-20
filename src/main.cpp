#include <iostream>
#include "parser.hpp"
#include "functions.h"
using namespace std;

std::map<std::string, Function> map_functions;
std::map<std::string, double> map_variables;


int main()
{
    def_functions();
    def_variables();
    Parser parser;
    while(true)
    {
        parser = Parser();
        string expr;
        string::iterator it_expr;
        cout<<">";
        getline(cin,expr);
        it_expr = expr.begin();
        if(expr == ";"){return 0;}
        parser.parse(expr);
        expr.clear();
    }





    return 0;
}

