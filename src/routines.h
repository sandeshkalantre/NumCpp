/*
This file contains the main routines of the program in the routine namespace.
See routines.cpp for their implementation
*/

#ifndef ROUTINES_H
#define ROUTINES_H

#include "parser.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <time.h>

//Number class defined in parser.hpp
class Number;
//defines the routines
void def_routines();

namespace routines
{
    Number integrate1(std::string function_name, Number a, Number b);
    Number integrate2(std::string function_name, Number a, Number b);
    Number integrate3(std::string function_name, Number a, Number b);
    Number newton(std::string function_name, Number a);
    Number bisection(std::string function_name, Number a, Number b);
    Number differentiate(std::string function_name, Number a);
    Number integrate_rect(std::string function_name,Number a,Number b,Number c,Number d);
    Number integrate2d(std::string function_name,Number a,Number b,std::vector<std::string> aux_arguments);
}

#endif
