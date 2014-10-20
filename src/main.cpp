#include <iostream>
#include "parser.hpp"
using namespace std;

std::map<std::string, Function> map_functions;
std::map<std::string, double> map_variables;
//defines the standard functions and loads them in map_functions
void def_functions();
//defines the standard variables and loads then in map_variables
void def_variables();


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

//defines the standard functions and loads them in map_function
void def_functions()
{
//Function declarations
Function ADD;
ADD.function_name = "ADD";
ADD.num_arguments = 2;
ADD.standard = true;
map_functions[ADD.function_name] = ADD;


Function SUBTRACT;
SUBTRACT.function_name = "SUBTRACT";
SUBTRACT.num_arguments = 2;
SUBTRACT.standard = true;
map_functions[SUBTRACT.function_name] = SUBTRACT;


Function MULTIPLY;
MULTIPLY.function_name = "MULTIPLY";
MULTIPLY.num_arguments = 2;
MULTIPLY.standard = true;
map_functions[MULTIPLY.function_name] = MULTIPLY;


Function DIVIDE;
DIVIDE.function_name = "DIVIDE";
DIVIDE.num_arguments = 2;
DIVIDE.standard = true;
map_functions[DIVIDE.function_name] = DIVIDE;



Function MODULUS;
MODULUS.function_name = "MODULUS";
MODULUS.num_arguments = 2;
MODULUS.standard = true;
map_functions[MODULUS.function_name] = MODULUS;

Function FACTORIAL;
FACTORIAL.function_name = "FACTORIAL";
FACTORIAL.num_arguments = 2;
FACTORIAL.standard = true;
map_functions[FACTORIAL.function_name] = FACTORIAL;

Function POWER;
POWER.function_name = "POWER";
POWER.num_arguments = 2;
POWER.standard = true;
map_functions[POWER.function_name] = POWER;

return;
}

void def_variables()
{
map_variables["e"] = 2.732;
map_variables["pi"] = 3.14159;
}

