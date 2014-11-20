#include "parser.hpp"

//16 decimal digits of constants are stored
void def_variables()
{
    Number e;
    mpfr_set_str(e.value,"2.7182818284590452",BASE,RND_MODE);
    map_variables["math.e"] = e;

    Number pi;
    mpfr_set_str(pi.value,"3.1415926535897932",BASE,RND_MODE);
    map_variables["math.pi"] = pi;

    Number phi;
    mpfr_set_str(phi.value,"1.6180339887498942",BASE,RND_MODE);
    map_variables["math.phi"] = phi;

    Number c(299792458.0);
    map_variables["phy.c"] = c;

    map_variables["_"] = Number(0.0);

    return;
}

