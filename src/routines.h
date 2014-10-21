#ifndef ROUTINES_H
#define ROUTINES_H

#include <iostream>
#include "parser.hpp"

//defines the routines
void def_routines();

namespace routines
{
    double integrate(std::string function_name, double a, double b);
    double differentiate(std::string function_name, double a);
}

#endif
