#include "functions.h"

//the operators for Number objects have been overloaded so
//as to simplify the implementation
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
        //handle division by zero error
        //returns nan(NOT A NUMBER) for such a case
        try
        {
            if(b == Number(0.0))
            {
                throw DIVISION_BY_ZERO;
            }
            return a / b;
        }
        catch(const char *str)
        {
            std::cout<<str<<std::endl;
            Number result;
            mpfr_set_nan(result.value);
            return result;
        }
    }
    Number modulus1(Number a, Number b)
    {
        return a % b;
    }
    Number scientific(Number a, Number b)
    {
        Number base(BASE);
        return a * (base ^ b);
    }
    Number factorial(Number a)
    {
        try
        {
            //if the number is negative
            if(a < Number(0.0))
            {
                throw NEGATIVE_FACTORIAL;
            }
            Number result;
            unsigned long _a = mpfr_get_ui(a.value,RND_MODE);
            mpfr_fac_ui(result.value,_a,RND_MODE);
            return result;
        }
        catch(const char *str)
        {
            std::cout<<NEGATIVE_FACTORIAL<<std::endl;
            Number result;
            mpfr_set_nan(result.value);
            return result;
        }

    }
    Number power(Number a, Number b)
    {
        return a ^ b;
    }
    Number sin(Number a)
    {
        Number result;
        mpfr_sin(result.value,a.value,RND_MODE);
        return result;
    }
    Number cos(Number a)
    {
        Number result;
        mpfr_cos(result.value,a.value,RND_MODE);
        return result;
    }
    Number tan(Number a)
    {
        Number result;
        mpfr_tan(result.value,a.value,RND_MODE);
        return result;
    }
    Number sec(Number a)
    {
        Number result;
        mpfr_sec(result.value,a.value,RND_MODE);
        return result;
    }
    Number csc(Number a)
    {
        Number result;
        mpfr_csc(result.value,a.value,RND_MODE);
        return result;
    }
    Number cot(Number a)
    {
        Number result;
        mpfr_cot(result.value,a.value,RND_MODE);
        return result;
    }
    Number asin(Number a)
    {
        Number result;
        mpfr_asin(result.value,a.value,RND_MODE);
        return result;
    }
    Number acos(Number a)
    {
        Number result;
        mpfr_acos(result.value,a.value,RND_MODE);
        return result;
    }
    Number atan(Number a)
    {
        Number result;
        mpfr_atan(result.value,a.value,RND_MODE);
        return result;
    }
    Number sinh(Number a)
    {
        Number result;
        mpfr_sinh(result.value,a.value,RND_MODE);
        return result;
    }
    Number cosh(Number a)
    {
        Number result;
        mpfr_cosh(result.value,a.value,RND_MODE);
        return result;
    }
    Number tanh(Number a)
    {
        Number result;
        mpfr_tanh(result.value,a.value,RND_MODE);
        return result;
    }
    Number sech(Number a)
    {
        Number result;
        mpfr_sech(result.value,a.value,RND_MODE);
        return result;
    }
    Number csch(Number a)
    {
        Number result;
        mpfr_csch(result.value,a.value,RND_MODE);
        return result;
    }
    Number coth(Number a)
    {
        Number result;
        mpfr_coth(result.value,a.value,RND_MODE);
        return result;
    }
    Number asinh(Number a)
    {
        Number result;
        mpfr_asinh(result.value,a.value,RND_MODE);
        return result;
    }
    Number acosh(Number a)
    {
        Number result;
        mpfr_acosh(result.value,a.value,RND_MODE);
        return result;
    }
    Number atanh(Number a)
    {
        Number result;
        mpfr_atanh(result.value,a.value,RND_MODE);
        return result;
    }
    Number atan2(Number a, Number b)
    {
        Number result;
        mpfr_atan2(result.value,a.value,b.value,RND_MODE);
        return result;
    }
    //log (1+x)
    Number log1p(Number a)
    {
        Number result;
        mpfr_log1p(result.value, a.value, RND_MODE);
        return result;
    }
    //e^x - 1
    Number expm1(Number a)
    {
        Number result;
        mpfr_expm1(result.value, a.value,RND_MODE);
        return result;
    }
    //exponential integral (only for +ve)
    Number eint(Number a)
    {
        Number result;
        mpfr_eint(result.value, a.value,RND_MODE);
        return result;
    }
    //dilogarithm
    Number li2(Number a)
    {
        Number result;
        mpfr_li2(result.value, a.value,RND_MODE);
        return result;
    }
    Number gamma(Number a)
    {
        Number result;
        mpfr_gamma(result.value, a.value,RND_MODE);
        return result;
    }
    Number lngamma(Number a)
    {
        Number result;
        mpfr_lngamma(result.value, a.value,RND_MODE);
        return result;
    }
    //Digamma function aka Psi
    Number digamma(Number a)
    {
        Number result;
        mpfr_digamma(result.value, a.value,RND_MODE);
        return result;
    }
    Number zeta(Number a)
    {
        Number result;
        mpfr_zeta(result.value, a.value,RND_MODE);
        return result;
    }
    //error function
    Number erf(Number a)
    {
        Number result;
        mpfr_erf(result.value, a.value,RND_MODE);
        return result;
    }
    //complement of error function
    Number erfc(Number a)
    {
        Number result;
        mpfr_erfc(result.value, a.value,RND_MODE);
        return result;
    }
    //First kind of bessel functions for 0,1,n
    Number j0(Number a)
    {
        Number result;
        mpfr_j0(result.value, a.value,RND_MODE);
        return result;
    }
    Number j1(Number a)
    {
        Number result;
        mpfr_j1(result.value, a.value,RND_MODE);
        return result;
    }
    Number jn(Number a,Number b)
    {
        long n = mpfr_get_si(b.value,RND_MODE);
        Number result;
        mpfr_jn(result.value, n ,a.value,RND_MODE);
        return result;
    }
    //Second kind of bessel function for 0,1,n
    Number y0(Number a)
    {
        Number result;
        mpfr_y0(result.value, a.value,RND_MODE);
        return result;
    }
    Number y1(Number a)
    {
        Number result;
        mpfr_y1(result.value, a.value,RND_MODE);
        return result;
    }
    Number yn(Number a,Number b)
    {
        long n = mpfr_get_si(b.value,RND_MODE);
        Number result;
        mpfr_yn(result.value,n, a.value,RND_MODE);
        return result;
    }
    //arithmatic geometric mean of a and b
    Number agm(Number a, Number b)
    {
        Number result;
        mpfr_agm(result.value, a.value, b.value, RND_MODE);
        return result;
    }
    //Euclidean norm of a and b sqrt(a^2 + b^2)
    Number hypot(Number a, Number b)
    {
        Number result;
        mpfr_hypot(result.value, a.value ,b.value, RND_MODE);
        return result;
    }
    //Airy function,
    //mpfr implementation is incorrect
    /*
    Number ai(Number a)
    {
        Number result;
        mpfr_ai(result.value, a.value,RND_MODE);
        return result;
    }
    */
}

//defines the standard functions and loads them in map_functions
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

    Function SEC;
    SEC.function_name = "sec";
    SEC.num_arguments = 1;
    SEC.standard = true;
    map_functions[SEC.function_name] = SEC;

    Function CSC;
    CSC.function_name = "csc";
    CSC.num_arguments = 1;
    CSC.standard = true;
    map_functions[CSC.function_name] = CSC;

    Function COT;
    COT.function_name = "cot";
    COT.num_arguments = 1;
    COT.standard = true;
    map_functions[COT.function_name] = COT;

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

    Function SINH;
    SINH.function_name = "sinh";
    SINH.num_arguments = 1;
    SINH.standard = true;
    map_functions[SINH.function_name] = SINH;

    Function COSH;
    COSH.function_name = "cosh";
    COSH.num_arguments = 1;
    COSH.standard = true;
    map_functions[COSH.function_name] = COSH;

    Function TANH;
    TANH.function_name = "tanh";
    TANH.num_arguments = 1;
    TANH.standard = true;
    map_functions[TANH.function_name] = TANH;

    Function SECH;
    SECH.function_name = "sech";
    SECH.num_arguments = 1;
    SECH.standard = true;
    map_functions[SECH.function_name] = SECH;

    Function CSCH;
    CSCH.function_name = "csch";
    CSCH.num_arguments = 1;
    CSCH.standard = true;
    map_functions[CSCH.function_name] = CSCH;

    Function COTH;
    COTH.function_name = "coth";
    COTH.num_arguments = 1;
    COTH.standard = true;
    map_functions[COTH.function_name] = COTH;

    Function ASINH;
    ASINH.function_name = "asinh";
    ASINH.num_arguments = 1;
    ASINH.standard = true;
    map_functions[ASINH.function_name] = ASINH;

    Function ACOSH;
    ACOSH.function_name = "acosh";
    ACOSH.num_arguments = 1;
    ACOSH.standard = true;
    map_functions[ACOSH.function_name] = ACOSH;

    Function ATANH;
    ATANH.function_name = "atanh";
    ATANH.num_arguments = 1;
    ATANH.standard = true;
    map_functions[ATANH.function_name] = ATANH;

    Function ATAN2;
    ATAN2.function_name = "atan2";
    ATAN2.num_arguments = 2;
    ATAN2.standard = true;
    map_functions[ATAN2.function_name] = ATAN2;

    Function LOG1P;
    LOG1P.function_name = "log1p";
    LOG1P.num_arguments = 1;
    LOG1P.standard = true;
    map_functions[LOG1P.function_name] = LOG1P;

    Function EXPM1;
    EXPM1.function_name = "expm1";
    EXPM1.num_arguments = 1;
    EXPM1.standard = true;
    map_functions[EXPM1.function_name] = EXPM1;

    Function EINT;
    EINT.function_name = "eint";
    EINT.num_arguments = 1;
    EINT.standard = true;
    map_functions[EINT.function_name] = EINT;

    Function LI2;
    LI2.function_name = "li2";
    LI2.num_arguments = 1;
    LI2.standard = true;
    map_functions[LI2.function_name] = LI2;

    Function GAMMA;
    GAMMA.function_name = "gamma";
    GAMMA.num_arguments = 1;
    GAMMA.standard = true;
    map_functions[GAMMA.function_name] = GAMMA;

    Function LNGAMMA;
    LNGAMMA.function_name = "lngamma";
    LNGAMMA.num_arguments = 1;
    LNGAMMA.standard = true;
    map_functions[LNGAMMA.function_name] = LNGAMMA;

    Function DIGAMMA;
    DIGAMMA.function_name = "digamma";
    DIGAMMA.num_arguments = 1;
    DIGAMMA.standard = true;
    map_functions[DIGAMMA.function_name] = DIGAMMA;

    Function ZETA;
    ZETA.function_name = "zeta";
    ZETA.num_arguments = 1;
    ZETA.standard = true;
    map_functions[ZETA.function_name] = ZETA;

    Function ERF;
    ERF.function_name = "erf";
    ERF.num_arguments = 1;
    ERF.standard = true;
    map_functions[ERF.function_name] = ERF;

    Function ERFC;
    ERFC.function_name = "erfc";
    ERFC.num_arguments = 1;
    ERFC.standard = true;
    map_functions[ERFC.function_name] = ERFC;

    Function J0;
    J0.function_name = "j0";
    J0.num_arguments = 1;
    J0.standard = true;
    map_functions[J0.function_name] = J0;

    Function J1;
    J1.function_name = "j1";
    J1.num_arguments = 1;
    J1.standard = true;
    map_functions[J1.function_name] = J1;

    Function JN;
    JN.function_name = "jn";
    JN.num_arguments = 2;
    JN.standard = true;
    map_functions[JN.function_name] = JN;

    Function Y0;
    Y0.function_name = "y0";
    Y0.num_arguments = 1;
    Y0.standard = true;
    map_functions[Y0.function_name] = Y0;

    Function Y1;
    Y1.function_name = "y1";
    Y1.num_arguments = 1;
    Y1.standard = true;
    map_functions[Y1.function_name] = Y1;

    Function YN;
    YN.function_name = "yn";
    YN.num_arguments = 2;
    YN.standard = true;
    map_functions[YN.function_name] = YN;

    Function AGM;
    AGM.function_name = "agm";
    AGM.num_arguments = 2;
    AGM.standard = true;
    map_functions[AGM.function_name] = AGM;

    Function HYPOT;
    HYPOT.function_name = "hypot";
    HYPOT.num_arguments = 2;
    HYPOT.standard = true;
    map_functions[HYPOT.function_name] = HYPOT;

    /*
    Function AI;
    AI.function_name = "ai";
    AI.num_arguments = 1;
    map_functions[AI.function_name] = AI;
    */
    return;
}
