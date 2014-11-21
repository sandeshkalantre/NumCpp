#include "parser.hpp"

//16 decimal digits of mathematical constants are stored
//for physical constants CODATA 2010 values were used
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

    Number mu_0(1.2566370614e-12);
    map_variables["phy.mu_0"] = mu_0;

    Number epsilon_0(8.854187817e-12);
    map_variables["phy.epsilon_0"] = epsilon_0;

    Number h(6.62606957e-34);
    map_variables["phy.h"] = h;

    Number hbar(1.054571726e-34);
    map_variables["phy.hbar"] = hbar;

    //for the sake of it
    Number hcut(1.054571726e-34);
    map_variables["phy.hcut"] = hcut;

    Number G(6.67384e-11);
    map_variables["phy.G"] = G;

    Number g(9.80665 );
    map_variables["phy.g"] = g;

    Number phye(1.602176565e-19);
    map_variables["phy.e"] = phye;

    Number R(8.3144621);
    map_variables["phy.R"] = R;

    Number alpha(7.29735257e-3);
    map_variables["phy.alpha"] = alpha;

    Number N_A(6.02214129e+23);
    map_variables["phy.N_A"] = N_A;

    Number k(1.3806488e-23);
    map_variables["phy.k"] = k;

    Number sigma(6.97787e-16);
    map_variables["phy.sigma"] = sigma;

    Number Wein(0.0028977721);
    map_variables["phy.Wein"] = Wein;

    Number Rydberg(10973731.5685);
    map_variables["phy.Rydberg"] = Rydberg;

    Number m_e(9.10938291e-31);
    map_variables["phy.m_e"] = m_e;

    Number m_p(1.672621777e-27);
    map_variables["phy.m_p"] = m_p;

    Number m_n(1.674927351e-27);
    map_variables["phy.m_n"] = m_n;

    map_variables["_"] = Number(0.0);

    return;
}
