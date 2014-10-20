#include "parser.hpp"
#include <cmath>

//defines the standard functions and loads them in map_functions
void def_functions();
//defines the standard variables and loads then in map_variables
void def_variables();

namespace std_functions
{
    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);
    double modulus1(double a, double b);
    double factorial(double a, double b);
    double power(double a, double b);
}
