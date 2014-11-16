#include "functions.h"
#include "parser.hpp"


namespace std_functions
{
    Number add(Number a, Number b)
    {
        return a + b;
    }
    Number subtract(Number a, Number b)
    {
        return a - b;
    }
    Number unary_minus(Number a)
    {
        return (-a);
    }
    Number multiply(Number a,Number b)
    {
        return a * b;
    }
    Number divide(Number a, Number b)
    {
        if(mpfr_zero_p(b.value) != 0)
        {
            std::cout<<"Divide by zero error."<<std::endl;
            Number result;
            mpfr_set_nan(result.value);
            return result;
        }
        return a / b;
    }
    Number modulus1(Number a, Number b)
    {
        return a % b;
    }
    Number scientific(Number a, Number b)
    {
        Number base;
        mpfr_set_ui(base.value,BASE,MPFR_RNDN);
        return a * (base ^ b);
    }
    Number factorial(Number a)
    {
        //if the number is negative
        /*
        if(a.return_value() < 0)
        {
            std::cout<<"Factorial of negative number does not exist.Use gamma(x)."<<std::endl;
            return NAN;
        }
        int num = (int) a.return_value();
        int result = 1;
        while(num > 0)
        {
            result *= num;
            num--;
        }
        return Number(result);
        */
        Number result;
        unsigned long _a = mpfr_get_ui(a.value,MPFR_RNDN);
        mpfr_fac_ui(result.value,_a,MPFR_RNDN);
        return result;
    }
    Number power(Number a, Number b)
    {
        return a ^ b;
    }
    Number sin(Number a)
    {
        Number result;
        mpfr_sin(result.value,a.value,MPFR_RNDN);
        return result;
    }
    Number cos(Number a)
    {
        Number result;
        mpfr_cos(result.value,a.value,MPFR_RNDN);
        return result;
    }
    Number tan(Number a)
    {
        Number result;
        mpfr_tan(result.value,a.value,MPFR_RNDN);
        return result;
    }
    Number asin(Number a)
    {
        Number result;
        mpfr_asin(result.value,a.value,MPFR_RNDN);
        return result;
    }
    Number acos(Number a)
    {
        Number result;
        mpfr_acos(result.value,a.value,MPFR_RNDN);
        return result;
    }
    Number atan(Number a)
    {
        Number result;
        mpfr_atan(result.value,a.value,MPFR_RNDN);
        return result;
    }
    Number atan2(Number a, Number b)
    {
        Number result;
        mpfr_atan2(result.value,a.value,b.value,MPFR_RNDN);
        return result;
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



