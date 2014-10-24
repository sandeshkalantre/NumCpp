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
    double unary_minus(double a)
    {
        return (-1 * a);
    }
    double multiply(double a, double b)
    {
        return a * b;
    }
    double divide(double a, double b)
    {
        if(b == 0)
        {
            std::cout<<"Divide by zero error."<<std::endl;
            return NAN;
        }
        return a / b;
    }
    double modulus1(double a, double b)
    {
        return (int)a % (int)b;
    }
    double scientific(double a, double b)
    {
        return a * pow(10 , b);
    }
    double factorial(double a)
    {
        //if the number is negative
        if(a < 0)
        {
            std::cout<<"Factorial of negative number does not exist.Use gamma(x)."<<std::endl;
            return NAN;
        }
        int num = (int) a;
        int result = 1;
        while(num > 0)
        {
            result *= num;
            num--;
        }
        return result;
    }
    double power(double a, double b)
    {
        return pow(a , b);
    }
    double sin(double a)
    {
        return std::sin(a);
    }
    double cos(double a)
    {
        return std::cos(a);
    }
    double tan(double a)
    {
        return std::tan(a);
    }
    double asin(double a)
    {
        return std::asin(a);
    }
    double acos(double a)
    {
        return std::acos(a);
    }
    double atan(double a)
    {
        return std::atan(a);
    }
    double atan2(double a, double b)
    {
        return std::atan2(a, b);
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

    Function UNARY_MINUS;
    UNARY_MINUS.function_name = "UNARY_MINUS";
    UNARY_MINUS.num_arguments = 1;
    UNARY_MINUS.standard = true;
    map_functions[UNARY_MINUS.function_name] = UNARY_MINUS;

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

    Function E;
    E.function_name = "E";
    E.num_arguments = 2;
    E.standard = true;
    map_functions[E.function_name] = E;

    Function FACTORIAL;
    FACTORIAL.function_name = "FACTORIAL";
    FACTORIAL.num_arguments = 1;
    FACTORIAL.standard = true;
    map_functions[FACTORIAL.function_name] = FACTORIAL;

    Function POWER;
    POWER.function_name = "POWER";
    POWER.num_arguments = 2;
    POWER.standard = true;
    map_functions[POWER.function_name] = POWER;

    Function SIN;
    SIN.function_name = "sin";
    SIN.num_arguments = 1;
    SIN.standard = true;
    map_functions[SIN.function_name] = SIN;

    Function COS;
    COS.function_name = "cos";
    COS.num_arguments = 1;
    COS.standard = true;
    map_functions[COS.function_name] = COS;

    Function TAN;
    TAN.function_name = "tan";
    TAN.num_arguments = 1;
    TAN.standard = true;
    map_functions[TAN.function_name] = TAN;

    Function ASIN;
    ASIN.function_name = "asin";
    ASIN.num_arguments = 1;
    ASIN.standard = true;
    map_functions[ASIN.function_name] = ASIN;

    Function ACOS;
    ACOS.function_name = "acos";
    ACOS.num_arguments = 1;
    ACOS.standard = true;
    map_functions[ACOS.function_name] = ACOS;

    Function ATAN;
    ATAN.function_name = "atan";
    ATAN.num_arguments = 1;
    ATAN.standard = true;
    map_functions[ATAN.function_name] = ATAN;

    Function ATAN2;
    ATAN2.function_name = "atan2";
    ATAN2.num_arguments = 2;
    ATAN2.standard = true;
    map_functions[ATAN2.function_name] = ATAN2;

    return;
}



