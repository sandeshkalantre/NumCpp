#include "routines.h"

namespace routines
{
    double integrate(std::string function_name, double a, double b)
    {
        return 1;
    }
    double differentiate(std::string function_name, double a)
    {
        return 2;
    }

}




void def_routines()
{
    Routine INTEGRATE;
    INTEGRATE.routine_name = "integrate";
    INTEGRATE.num_arguments = 2;
    map_routines[INTEGRATE.routine_name] = INTEGRATE;

    Routine DIFFERENTIATE;
    INTEGRATE.routine_name = "differentiate";
    INTEGRATE.num_arguments = 2;
    map_routines[INTEGRATE.routine_name] = INTEGRATE;

    return;
}

