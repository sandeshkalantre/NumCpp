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
    Number integrate1(std::string function_name, Number a, Number b);
    Number integrate2(std::string function_name, Number a, Number b);
    Number integrate3(std::string function_name, Number a, Number b);
    Number newton(std::string function_name, Number a);
    Number bisection(std::string function_name, Number a, Number b);
    Number differentiate(std::string function_name, Number a);
}

#endif
