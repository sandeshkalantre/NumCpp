#include "parser.hpp"

void def_variables()
{
    map_variables["e"] = Number(2.713);

    Number temp;
    mpfr_const_pi(temp.value,MPFR_RNDN);
    map_variables["pi"] = temp;

    map_variables["_"] = Number(0.0);

    return;
}

