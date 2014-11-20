/*
Contains the standard mathematical functions used in the program
in the namespace std_functions

Also contains the function def_functions which load the standard function
at the start of program execution
*/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "parser.hpp"
#include "errors.h"

//class from parser.hpp
class Number;

//defines the standard functions and loads them in map_functions
//called from main()
void def_functions();

namespace std_functions
{
    //see functions.cpp for the working of each function
    Number add(Number a, Number b);
    Number subtract(Number a, Number b);
    Number unary_minus(Number a);
    Number multiply(Number a, Number b);
    Number divide(Number a, Number b);
    //modulus is some keyword so modulus1 is used as the name
    Number modulus1(Number a, Number b);
    Number scientific(Number a, Number b);
    Number factorial(Number a);
    Number power(Number a, Number b);
    Number sin(Number a);
    Number cos(Number a);
    Number tan(Number a);
    Number sec(Number a);
    Number csc(Number a);
    Number cot(Number a);
    Number asin(Number a);
    Number acos(Number a);
    Number atan(Number a);
    Number sinh(Number a);
    Number cosh(Number a);
    Number tanh(Number a);
    Number sech(Number a);
    Number csch(Number a);
    Number coth(Number a);
    Number asinh(Number a);
    Number acosh(Number a);
    Number atanh(Number a);
    Number atan2(Number a, Number b);
    Number log1p(Number a);
    Number expm1(Number a);
    Number eint(Number a);
    Number li2(Number a);
    Number gamma(Number a);
    Number lngamma(Number a);
    Number digamma(Number a);
    Number zeta(Number a);
    Number erf(Number a);
    Number erfc(Number a);
    Number j0(Number a);
    Number j1(Number a);
    Number jn(Number a,Number b);
    Number y0(Number a);
    Number y1(Number a);
    Number yn(Number a,Number b);
    Number agm(Number a, Number b);
    Number hypot(Number a, Number b);
    Number ai(Number a);
}

#endif
