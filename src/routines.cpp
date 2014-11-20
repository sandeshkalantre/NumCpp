#include "routines.h"

namespace routines
{
    //Riemann integration with evaluation at midpoint of the subinterval

    Number integrate1(std::string function_name, Number a, Number b)
    {
        Number sign(1.0); Number temp;


        if(b<a)
        {
            temp = b;
            b = a;
            a = temp;
            sign = Number(-1);
        }
        //h is the width of the subinterval
        Number h((b-a)/Number(100000.0));
        //x starts at the midpoint of the first subinterval (a, a+h)
        Number x = a + h/2 ;
        Number integral(0.0);
        std::vector<Number> arguments(1);


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
    Number integrate2(std::string function_name, Number a, Number b)
    {
        Number sign (1.0); Number temp;


        if(b<a)
        {
            temp = b;
            b = a;
            a = temp;
            sign = Number(-1);
        }

        Number h((b-a)/(Number)10000) ;
        Number x (a);
        Number integral(0.0);
        Number t;
        std::vector<Number> arguments(1);
        srand(time(NULL));
        while(x<b)
        {
            // t is a Number from 0 to h
            t = h*((Number)rand()/(Number)RAND_MAX);

            arguments[0] = x + t;
            integral += map_functions[function_name].evaluate(arguments);
            x += h;
        }
        integral *= sign*h;
        return integral;
    }


    //Monte Carlo method
    Number integrate3(std::string function_name, Number a, Number b)
    {
        Number sign (1.0); Number temp;

        if(b<a)
        {
            temp = b;
            b = a;
            a = temp;
            sign = Number(-1);
        }

        Number x = a;
        Number function_value;
        Number width (b-a);
        Number h(width/(Number(10000)));
        std::vector<Number> arguments(1);
        arguments[0] = x;
        Number maximum = map_functions[function_name].evaluate(arguments);
        Number minimum = map_functions[function_name].evaluate(arguments);

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

        Number i(0.0);
        Number y;
        Number inside_pts (0.0);
        Number height (maximum - minimum);
        Number total_pts = Number(100000)*width*height;
        while( (i+=1) < total_pts )
        {
            x = Number(1.0*rand()/RAND_MAX);
            y = Number(1.0*rand()/RAND_MAX);
            arguments[0] = x;
            function_value = map_functions[function_name].evaluate(arguments);
            if(function_value > y && y > 0 )
                inside_pts+=1;
            else if(function_value < y && y < 0)
                inside_pts-=1;
        }
        Number integral = sign*width*height*inside_pts/total_pts;
        return integral;
    }

    Number integrate_rect(std::string function_name, Number a, Number b, Number c, Number d)
    {
        Number sign(1); Number temp;
        if(b<a)
        {
            temp = a;
            a = b;
            b = temp;
            sign=sign*Number(-1);
        }
        if(d<c)
        {
            temp = c;
            c = d;
            d = temp;
            sign=sign*Number(-1);
        }
        Number h1 ((b-a)/Number(100));
        Number h2 ((d-c)/Number(100));
        Number x(a + h1/Number(2));
        Number integral(0.0);
        std::vector<Number> arguments(2);
        while(x<b)
        {
            arguments[0]=x;
            Number y(c+ h2/Number(2));
            while (y<d)
            {
                arguments[1] = y;
                integral += map_functions[function_name].evaluate(arguments);
                y += h2;
            }
            x += h1;
        }
        integral*=(sign*h1*h2);
        return integral;
    }
    Number integrate2d(std::string function_name, Number a, Number b,std::vector<std::string> aux_arguments)
    {
        std::string function_name_1 = aux_arguments[0];
        std::string function_name_2 = aux_arguments[1];
        Number sign(1); Number temp;
        if(b<a)
        {
            temp = a;
            a = b;
            b = temp;
            sign=sign*Number(-1);
        }
        Number h1 ((b-a)/Number(100));
        Number x(a + h1/Number(2));
        Number integral(0.0);
        Number h2, y;
        Number sign1(1);
        Number c,d;
        std::vector<Number> limits(1);
        std::vector<Number> arguments(2);
        while(x<b)
        {
            arguments[0] = x;
            limits[0] = x;
            c = map_functions[function_name_1].evaluate(limits);
            d = map_functions[function_name_2].evaluate(limits);
            if(d<c)
            {
                temp = c;
                c = d;
                d = temp;
                sign1 = sign1*Number(-1);
            }
            h2 = (d-c)/Number(100);
            y = c+ h2/Number(2);
            while (y<d)
            {
                arguments[1] = y;
                integral += (h2*sign1*map_functions[function_name].evaluate(arguments));
                y += h2;
            }
            x += h1;

        }
        integral*=(sign*h1);
        return integral;
    }

    Number differentiate(std::string function_name, Number a)

    {
        std::vector<Number> arguments (1);
        arguments[0] = a;
    	//IS THIS A GOOD VALUE?
        Number h (0.000001);
        //evaluate the value of the function just at the given point
        Number fx= map_functions[function_name].evaluate(arguments);
        //evaluate the value of the function just to the right of the given point
        arguments[0] = a + h;
        Number fx1 (map_functions[function_name].evaluate(arguments));
        Number slope ((fx1 - fx )/ h);
        return slope;

    }


    //Newton Raphson Method
    Number newton(std::string function_name, Number x)
    {
        std::vector<Number> arguments(1);
        //x is the initial guess of the root given by the user
        arguments[0] = x;
        Number func_val = map_functions[function_name].evaluate(arguments);
        //defining an approximate infinitesimal value to use as the epsilon about 0
        Number h (0.000001);
        Number slope (differentiate(function_name,x));
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
    Number bisection (std::string function_name, Number a, Number b)
    {
        std::vector<Number> arguments(1);
        arguments[0] = a;
        Number func_a = map_functions[function_name].evaluate(arguments);
        // if a is the root, return it
        if(func_a==0)
            return a;
        arguments[0] = b;
        Number func_b (map_functions[function_name].evaluate(arguments));
        // if b is the root, return it
        if (func_b==0)
            return b;
        //if function values of a and b are of the same sign, bisection method won't work
        //therefore print error message and return 42 to tell parser not to print anything
        if(func_a*func_b>0)
        {
             std::cout<<"Function values of given points must be of opposite sign. Unfortunately, given points have function value of the same sign."<<std::endl;
             suppress_zero = true;
             return Number(0.0);
        }
        //storing function value of midpoint of a and b
        arguments[0] = (a+b)/2;
        Number func_mp (map_functions[function_name].evaluate(arguments));
        //defining an approximate infinitesimal value to use as the epsilon about 0
        Number h (0.000001);
        while(func_mp>h || func_mp<-h)
        {
            //std::cout<<a<<" "<<b<<std::endl;
            //if function values of mp and a are of opposite signs, change limits to a and mp, since the root must lie between them
            if((func_mp< Number(0.0) && func_a>Number(0.0))||(func_mp > Number(0.0) && func_a < Number(0.0)))
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
    INTEGRATE.routine_help =
    "___________________________________________________________\n\n"
    "Syntax: integrate(left_limit, right_limit, function_name())\n"
    "___________________________________________________________\n\n"
    "This function will integrate the given function function_name over the given\n"
    "interval x=left_limit to x=right_limit, using the default integrating function,\n"
    "integrate.r_m. For more information on that, type help(integrate.r_m)\n";
     map_routines[INTEGRATE.routine_name] = INTEGRATE;


    Routine INTEGRATE1;
    INTEGRATE1.routine_name = "integrate.r_m";
    INTEGRATE1.num_arguments = 2;
    INTEGRATE1.routine_help =
    "Riemann method, evaluation at the midpoint\n"
    "_______________________________________________________________\n\n"
    "Syntax: integrate.r_m(left_limit, right_limit, function_name())\n"
    "_______________________________________________________________\n\n"
    "This function will integrate function_name from x = left_limit to x = right_limit.\n\n"
    "It approximates the area of the curve by evaluating the integral as an almost\n"
    "infinite sum of infinitesimally narrow rectangles.\n"
    "The width of each rectangle is a miniscule subdivision of the interval and the\n"
    "height is the function value at the midpoint of the subdivision.\n";
    map_routines[INTEGRATE1.routine_name] = INTEGRATE1;

    Routine INTEGRATE2;
    INTEGRATE2.routine_name = "integrate.r_t";
    INTEGRATE2.num_arguments = 2;
    INTEGRATE2.routine_help =
    "Riemann method, evaluation at a random tag\n"
    "_______________________________________________________________\n\n"
    "Syntax: integrate.r_t(left_limit, right_limit, function_name())\n"
    "_______________________________________________________________\n\n"
    "This function will integrate function_name from x=left_limit to x=right_limit.\n\n"
    "It approximates the area of the curve by evaluating the integral as an almost\n"
    "infinite sum of infinitesimally narrow rectangles.\n"
    "The width of each rectangle is a miniscule subdivision of the interval and the\n"
    "height is the function value at a random point in the subdivision.\n";
    map_routines[INTEGRATE2.routine_name] = INTEGRATE2;

    Routine INTEGRATE3;
    INTEGRATE3.routine_name = "integrate.mc";
    INTEGRATE3.num_arguments = 2;
    INTEGRATE3.routine_help =
    "Monte Carlo method\n"
    "______________________________________________________________\n\n"
    "Syntax: integrate.mc(left_limit, right_limit, function_name())\n"
    "______________________________________________________________\n\n"
    "This function will integrate function_name from x=left_limit to x = right_limit.\n"
    "It finds the maximum and minimum values of the curve in the given interval, so we\n"
    "know the rectangle bounding the curve.\n"
    "It then plots a large number of random points and checks whether it is inside or\n"
    "outside the area bounded by the curve and the x-axis.\n"
    "Thus if we divide number of points inside the curve by the total points plotted,\n"
    "accounting for negative area, and multiply that ratio by the total area of the\n"
    "rectangle, we can approximate the integral.\n"
    "This is not as accurate a method as Riemann method, refer to help(intgrate.r_m)\n"
    "for more.\n";
    map_routines[INTEGRATE3.routine_name] = INTEGRATE3;

    Routine INTEGRATE_RECT;
    INTEGRATE_RECT.routine_name = "integrate2d.rect";
    INTEGRATE_RECT.num_arguments = 4;
    INTEGRATE3.routine_help = "Integrating a function with 2 parameters over a rectangle";
    map_routines[INTEGRATE_RECT.routine_name] = INTEGRATE_RECT;

    Routine INTEGRATE_2D;
    INTEGRATE_2D.routine_name = "integrate2d.type1";
    INTEGRATE_2D.num_arguments = 2;
    INTEGRATE_2D.routine_help = "Integrates a function with 2 parameters from x=a to x=b and from y=f1(x) to y=f2(x)";
    map_routines[INTEGRATE_2D.routine_name] = INTEGRATE_2D;

    Routine DIFFERENTIATE;
    DIFFERENTIATE.routine_name = "differentiate";
    DIFFERENTIATE.num_arguments = 1;
    map_routines[DIFFERENTIATE.routine_name] = DIFFERENTIATE;

    Routine NEWTON;
    NEWTON.routine_name = "solve.n";
    NEWTON.num_arguments = 1;
    NEWTON.routine_help =
    "Newton's method\n"
    "_______________________________________\n\n"
    "Syntax: solve.n(guess, function_name())\n"
    "_______________________________________\n\n"
    "This will take a guess of the root from the user, and use it to find the actual\n"
    "root of the given function function_name.\n"
    "t evaluates the point of intersection of the tangent to the function at x = guess\n"
    "with the x-axis, which is an improved guess.\n"
    "It then keeps iterating this method to improve it's guess and eventually reaches\n"
    "the root.\n";
    map_routines[NEWTON.routine_name] = NEWTON;

    Routine BISECTION;
    BISECTION.routine_name = "solve.b";
    BISECTION.num_arguments = 2;
    BISECTION.routine_help =
    "Bisection method\n"
    "____________________________________________________\n\n"
    "Syntax: solve.b(left_end, right_end, function_name())\n"
    "____________________________________________________\n\n"
    "This takes in 2 points, having function values of opposite sign, and finds the\n"
    "root lying between them.\n"
    "It calculates the function value at the midpoint and out of the 2 subintervals\n"
    "created, chooses the one having endpoints with function values of opposite signs,\n"
    "thus implying that the root lies between them (assuming the given function is\n"
    "continuous)\n"
    "It iterates this method till it finds the root.\n"
    "This is not as accurate as Newton's method, refer to help(solve.n) for more.\n";
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
