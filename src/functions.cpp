#include "functions.h"

namespace std_functions
{
    double add(double a, double b)
    {
        return a + b;
    }
    double subtract(double a, double b)
    {
        return a - b;
    }
    double multiply(double a, double b)
    {
        return a * b;
    }
    double divide(double a, double b)
    {
        return a / b;
    }
    double modulus1(double a, double b)
    {
        return (int)a % (int)b;
    }
    double factorial(double a, double b)
    {
        //change this to factorial
        return a + b;
    }
    double power(double a, double b)
    {
        return pow(a , b);
    }
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



