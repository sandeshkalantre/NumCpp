#ifndef ROUTINES_H
#define ROUTINES_H

#include "parser.hpp"
#include <string>

//defines the routines
void def_routines();
//defines the arrays
void def_ndarrays();

namespace routines
{
    double integrate1(std::string function_name, double a, double b);
    double integrate2(std::string function_name, double a, double b);
    double integrate3(std::string function_name, double a, double b);
    double newton(std::string function_name, double a);
    double bisection(std::string function_name, double a, double b);
    double differentiate(std::string function_name, double a);
}

#endif
