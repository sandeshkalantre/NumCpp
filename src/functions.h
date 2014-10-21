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
    //modulus is some keyword so modulus1 is used as the name
    double modulus1(double a, double b);
    double factorial(double a);
    double power(double a, double b);
    double sin(double a);
    double cos(double a);
    double tan(double a);
    double asin(double a);
    double acos(double a);
    double atan(double a);
    double atan2(double a, double b);

}
