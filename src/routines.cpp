#include "routines.h"


namespace routines
{
    Number integrate(std::string function_name, Number a, Number b)
    {
        Number number;
        return number;
    }
    Number differentiate(std::string function_name, Number a)
    {
        std::vector<Number> arguments (1);
        arguments[0] = a;
	//IS THIS A GOOD VALUE?
        cppdouble _h;
        mpfr_set_d(_h,0.000001,MPFR_RNDN);
        Number h(_h);
        //evaluate the value of the function just at the given point
        Number fx= map_functions[function_name].evaluate(arguments);
        //evaluate the value of the function just to the right of the given point
        arguments[0] = a + h;
        Number fx1 = map_functions[function_name].evaluate(arguments);
        Number slope = (fx1 - fx )/ h;
        return slope;

    }


}




void def_routines()
{
    Routine INTEGRATE;
    INTEGRATE.routine_name = "integrate";
    INTEGRATE.num_arguments = 2;
    map_routines[INTEGRATE.routine_name] = INTEGRATE;

    Routine DIFFERENTIATE;
    DIFFERENTIATE.routine_name = "differentiate";
    DIFFERENTIATE.num_arguments = 1;
    map_routines[DIFFERENTIATE.routine_name] = DIFFERENTIATE;

    return;
}

void def_ndarrays()
{
    ndArray array_test;
    array_test.array_name = "YOLO";
    array_test.dim = 1;
    map_ndarrays[array_test.array_name] = array_test;

    return;

}
