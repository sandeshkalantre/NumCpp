#ifndef FUNCTIONS_H
#define FUNCTIONS_H

class Number;
//defines the standard functions and loads them in map_functions
void def_functions();

namespace std_functions
{
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
}

#endif
