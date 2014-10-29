<<<<<<< HEAD
#include <iostream>
#include <string>
#include <vector>
using namespace std;

double diff1 (std::string function_name, double a)
{
    std::vector<double> arguments (1);
    arguments[0] = a;
    double h = 0.000001;
    //evaluate the value of the function just at the given point
    double fx= map_functions[function_name].evaluate(arguments);
    //evaluate the value of the function just to the right of the given point
    arguments[0] = a+h;
    double fx1 = map_functions[function_name].evaluate(arguments);
    double slope = (fx1 - fx )/ h;
    return slope;
=======
#include "routines.h"

namespace routines
{
    double integrate(std::string function_name, double a, double b)
    {
        return 1;
    }
    double differentiate(std::string function_name, double a)
    {
        std::vector<double> arguments (1);
        arguments[0] = a;
	//IS THIS A GOOD VALUE?
        double h = 0.000001;
        //evaluate the value of the function just at the given point
        double fx= map_functions[function_name].evaluate(arguments);
        //evaluate the value of the function just to the right of the given point
        arguments[0] = a + h;
        double fx1 = map_functions[function_name].evaluate(arguments);
        double slope = (fx1 - fx )/ h;
        return slope;

    }

>>>>>>> upstream/master

}


<<<<<<< HEAD
double diff2 (std::string function_name, double a)
{
    std::vector<double> arguments (1);
    arguments[0] = a;
   //define h as tends to 0 somehow
   double h = 0.0000001;
   //value of the func. just to x's right
   arguments[0] = a+h;
   double fx2 = map_functions[function_name].evaluate(arguments);
   //value of the func. just to x's left
   arguments[0] = a-h;
   double fx1 =map_functions[function_name].evaluate(arguments);
   //central approx: f'(x) = (f(x+h) - f(x-h)) / 2h;
   double slope = ( fx2 - fx1 ) / (2*h) ;
   return slope;

}

double diff3 (std::string function_name, double a)
{
    std::vector<double> arguments (1);
    arguments[0] = a;
    //we will evaluate the function at 5 points around a.
    double h = 0.0000001;
    //value of function at distance 2h to the left of the point
    arguments[0] = a - (2*h);
    double fx1=  map_functions[function_name].evaluate(arguments);
    //value of function at distance h to the left of the point
    arguments[0] = a - h;
    double fx2 =  map_functions[function_name].evaluate(arguments);
    //value of function at distance 2h to the right of the point
    arguments[0] = a + h;
    double fx3 =  map_functions[function_name].evaluate(arguments);
    //value of function at distance h to the right of the point
    arguments[0] = a + (2*h);
    double fx4 =  map_functions[function_name].evaluate(arguments);
    double ans = ( fx1 - (8*fx2) + (8*fx3) - fx4 )/ (12*h) ;
    return ans;

}

=======


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
>>>>>>> upstream/master
