#include "routines.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#define SUPPRESS_ZERO 42
namespace routines
{
    //Riemann integration with evaluation at midpoint of the subinterval
    double integrate1(std::string function_name, double a, double b)
    {
        int sign = 1; double temp;
        if(b<a)
        {
            temp = b;
            b = a;
            a = temp;
            sign = -1;
        }
        //h is the width of the subinterval
        double h = (b-a)/100000;
        //x starts at the midpoint of the first subinterval (a, a+h)
        double x = a + h/2 ;
        double integral=0;
        std::vector<double> arguments(1);
        while(x<b)
        {
            arguments[0] = x;
            integral += map_functions[function_name].evaluate(arguments);
            x += h;
        }
        integral *= (sign*h);
        return integral;
    }
    //Riemann integration with evaluation at random tags in the subinterval
    double integrate2(std::string function_name, double a, double b)
    {
        int sign = 1; double temp;
        if(b<a)
        {
            temp = b;
            b = a;
            a = temp;
            sign = -1;
        }
        double h = (b-a)/10000 ;
        double x = a;
        double integral = 0;
        double t;
        std::vector<double> arguments(1);
        srand(time(NULL));
        while(x<b)
        {
            // t is a double from 0 to h
            t = h*((double)rand()/RAND_MAX);
            arguments[0] = x + t;
            integral += map_functions[function_name].evaluate(arguments);
            x += h;
        }
        integral *= sign*h;
        return integral;
    }
    //Monte Carlo method
    double integrate3(std::string function_name, double a, double b)
    {
        int sign = 1; double temp;
        if(b<a)
        {
            temp = b;
            b = a;
            a = temp;
            sign = -1;
        }
        double x = a;
        double function_value;
        double width = (b-a)/100000;
        double h = width;
        std::vector<double> arguments(1);
        arguments[0] = x;
        double maximum = map_functions[function_name].evaluate(arguments);
        double minimum = map_functions[function_name].evaluate(arguments);
        //finding maximum and minimum value of f in the given interval
        while(x<=b)
        {
            arguments[0] = x;
            function_value = map_functions[function_name].evaluate(arguments);
            if(function_value > maximum)
                maximum = function_value;
            else if(function_value < minimum)
                minimum = function_value;
            x+=h;
        }
        int i=0;
        double y;
        long int inside_pts = 0;
        double height = maximum - minimum;
        long int total_pts = (int)( 1000*width*height );
        while(i++ < total_pts )
        {
            x = (double)rand()/RAND_MAX;
            y = (double)rand()/RAND_MAX;
            arguments[0] = x;
            function_value = map_functions[function_name].evaluate(arguments);
            if(function_value > y && y > 0 )
                inside_pts++;
            else if(function_value < y && y < 0)
                inside_pts--;
        }
        double integral = sign*width*height*inside_pts/total_pts;
        return integral;
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
    //Newton Raphson Method
    double newton(std::string function_name, double x)
    {
        std::vector<double> arguments(1);
        //x is the initial guess of the root given by the user
        arguments[0] = x;
        double func_val = map_functions[function_name].evaluate(arguments);
        //defining an approximate infinitesimal value to use as the epsilon about 0
        double h = 0.000001;
        double slope = differentiate(function_name,x);
        while(func_val > h || func_val < -h)
        {
            //we better our approximation by finding where the tangent at x
            //intersects the x-axis
            if(slope <h &&slope>-h)
                x = x + h;
            //std::cout<<x<<std::endl;
            x = x - func_val/slope;
            slope = differentiate(function_name,x);
            arguments[0] = x;
            func_val = map_functions[function_name].evaluate(arguments);
        }
        return x;
    }
    //Bisection Method
    double bisection (std::string function_name, double a, double b)
    {
        std::vector<double> arguments(1);
        arguments[0] = a;
        double func_a = map_functions[function_name].evaluate(arguments);
        // if a is the root, return it
        if(func_a==0)
            return a;
        arguments[0] = b;
        double func_b = map_functions[function_name].evaluate(arguments);
        // if b is the root, return it
        if (func_b==0)
            return b;
        //if function values of a and b are of the same sign, bisection method won't work
        //therefore print error message and return 42 to tell parser not to print anything
        if(func_a*func_b>0)
        {
             std::cout<<"Function values of given points must be of opposite sign. Unfortunately, given points have function value of the same sign."<<std::endl;
             return SUPPRESS_ZERO;
        }
        //storing function value of midpoint of a and b
        arguments[0] = (a+b)/2;
        double func_mp = map_functions[function_name].evaluate(arguments);
        //defining an approximate infinitesimal value to use as the epsilon about 0
        double h = 0.000001;
        while(func_mp>h || func_mp<-h)
        {
            //std::cout<<a<<" "<<b<<std::endl;
            //if function values of mp and a are of opposite signs, change limits to a and mp, since the root must lie between them
            if((func_mp<0&&func_a>0)||(func_mp>0&&func_a<0))
            {
                b = (a+b)/2;
                arguments[0] = (a+b)/2;
                func_mp = map_functions[function_name].evaluate(arguments);
            }
            //if function values of mp and b are of opposite signs, change limits to mp and b, since the root must lie between them
            else
            {
                a = (a+b)/2;
                arguments[0] = (a+b)/2;
                func_mp = map_functions[function_name].evaluate(arguments);
            }
        }
        return (a+b)/2;
    }


}



void def_routines()
{
    Routine INTEGRATE;
    INTEGRATE.routine_name = "integrate";
    INTEGRATE.num_arguments = 2;
    map_routines[INTEGRATE.routine_name] = INTEGRATE;

    Routine INTEGRATE1;
    INTEGRATE1.routine_name = "integrate.r_m";
    INTEGRATE1.num_arguments = 2;
    map_routines[INTEGRATE1.routine_name] = INTEGRATE1;

    Routine INTEGRATE2;
    INTEGRATE2.routine_name = "integrate.r_t";
    INTEGRATE2.num_arguments = 2;
    map_routines[INTEGRATE2.routine_name] = INTEGRATE2;

    Routine INTEGRATE3;
    INTEGRATE3.routine_name = "integrate.mc";
    INTEGRATE3.num_arguments = 2;
    map_routines[INTEGRATE3.routine_name] = INTEGRATE3;

    Routine DIFFERENTIATE;
    DIFFERENTIATE.routine_name = "differentiate";
    DIFFERENTIATE.num_arguments = 1;
    map_routines[DIFFERENTIATE.routine_name] = DIFFERENTIATE;

    Routine NEWTON;
    NEWTON.routine_name = "solve.n";
    NEWTON.num_arguments = 1;
    map_routines[NEWTON.routine_name] = NEWTON;

    Routine BISECTION;
    BISECTION.routine_name = "solve.b";
    BISECTION.num_arguments = 2;
    map_routines[BISECTION.routine_name] = BISECTION;

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
