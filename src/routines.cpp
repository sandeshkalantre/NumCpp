#include "routines.h"

void check_nan_flag()
{
    if(mpfr_nanflag_p() != 0)
    {
        throw DOMAIN_ERROR;
        suppress_zero = true;
        mpfr_clear_nanflag();
    }
    return;
}

namespace routines
{

    Number integrate_rm_n(std::string function_name, Number a, Number b, Number n)
    {
        Number sign(1); Number temp;
        if(b<a)
        {
            temp = b;
            b = a;
            a = temp;
            sign = Number(-1);
        }
        Number num_divisions(n);
        //h is the width of the subinterval
        Number h((b-a)/num_divisions);
        //x starts at the midpoint of the first subinterval (a, a+h)
        Number x = a + h/Number(2) ;
        Number integral(0.0);
        std::vector<Number> arguments(1);


        while(x<b)
        {
            arguments[0] = x;
            integral += map_functions[function_name].evaluate(arguments);
            check_nan_flag();
            x += h;
        }
        integral *= (sign*h);
        return integral;
    }
    //Riemann integration with evaluation at midpoint of the subinterval

    Number integrate_rm(std::string function_name, Number a, Number b)
    {
        return integrate_rm_n(function_name, a, b, 100000);
        /*Number sign(1); Number temp;
        if(b<a)
        {
            temp = b;
            b = a;
            a = temp;
            sign = Number(-1);
        }
        Number num_divisions(100000);
        //h is the width of the subinterval
        Number h((b-a)/num_divisions);
        //x starts at the midpoint of the first subinterval (a, a+h)
        Number x = a + h/Number(2) ;
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
        */
    }
    //Riemann integration with evaluation at random tags in the subinterval
    Number integrate_rt(std::string function_name, Number a, Number b)
    {
        Number sign (1.0); Number temp;
        if(b<a)
        {
            temp = b;
            b = a;
            a = temp;
            sign = Number(-1);
        }
        Number num_divisions(100000);
        //h is the width of the subinterval
        Number h((b-a)/num_divisions);
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
            check_nan_flag();
            x += h;
        }
        integral *= sign*h;
        return integral;
    }
    //Monte Carlo method
    Number integrate_mc(std::string function_name, Number a, Number b)
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
        Number num_divisions(100000);
        //h is the width of the subinterval
        Number h(width/num_divisions);
        std::vector<Number> arguments(1);
        arguments[0] = x;
        Number maximum = map_functions[function_name].evaluate(arguments);
        Number minimum = map_functions[function_name].evaluate(arguments);
        check_nan_flag();
        //finding maximum and minimum value of f in the given interval
        while(x<=b)
        {
            arguments[0] = x;
            function_value = map_functions[function_name].evaluate(arguments);
            check_nan_flag();
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
            check_nan_flag();
            if(function_value > y && y > 0 )
                inside_pts+=1;
            else if(function_value < y && y < 0)
                inside_pts-=1;
        }
        Number integral = sign*width*height*inside_pts/total_pts;
        return integral;
    }
    Number integrate2d_rect(std::string function_name, Number a, Number b, Number c, Number d)
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
        Number num_divisions(1000);
        Number h1 ((b-a)/num_divisions);
        Number h2 ((d-c)/num_divisions);
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
                check_nan_flag();
                y += h2;
            }
            x += h1;
        }
        integral*=(sign*h1*h2);
        return integral;
    }
    Number integrate2d_type1(std::string function_name, Number a, Number b, std::vector<std::string>aux_arguments)
    {
        Number sign1(1); Number temp;
        if(b<a)
        {
            temp = a;
            a = b;
            b = temp;
            sign1 = Number(-1);
        }
        Number num_divisions(1000);
        Number h1 ((b-a)/num_divisions);
        Number x(a + h1/Number(2));
        Number integral(0.0);
        Number h2, y;
        Number sign2(1);
        std::string function_name_1 = aux_arguments[0];
        std::string function_name_2 = aux_arguments[1];
        std::vector<Number> limits(1);
        std::vector<Number> arguments(2);
        Number c, d;
        while(x<b)
        {
            arguments[0] = x;
            limits[0] = x;
            c = map_functions[function_name_1].evaluate(limits);
            d = map_functions[function_name_2].evaluate(limits);
            check_nan_flag();
            if(d<c)
            {
                temp = c;
                c = d;
                d = temp;
                sign2 = Number(-1);
            }
            h2 = (d-c)/num_divisions;
            y = c+ h2/Number(2);
            while (y<d)
            {
                arguments[1] = y;
                integral += (h2*sign2*map_functions[function_name].evaluate(arguments));
                check_nan_flag();
                y += h2;
            }
            x += h1;

        }
        integral*=(sign1*h1);
        return integral;
    }
    Number integrate2d_type2(std::string function_name, Number a, Number b, std::vector<std::string>aux_arguments)
    {
        Number sign1(1); Number temp;
        if(b<a)
        {
            temp = a;
            a = b;
            b = temp;
            sign1 = Number(-1);
        }
        Number num_divisions(1000);
        Number h1 ((b-a)/num_divisions);
        Number y(a + h1/Number(2));
        Number integral(0.0);
        Number h2, x;
        Number sign2(1);
        Number c, d;
        std::string function_name_1 = aux_arguments[0];
        std::string function_name_2 = aux_arguments[1];
        std::vector<Number> limits(1);
        std::vector<Number> arguments(2);
        while(y<b)
        {
            arguments[1] = y;
            limits[0] = y;
            c = map_functions[function_name_1].evaluate(limits);
            d = map_functions[function_name_2].evaluate(limits);
            check_nan_flag();
            if(d<c)
            {
                temp = c;
                c = d;
                d = temp;
                sign2 = Number(-1);
            }
            h2 = (d-c)/num_divisions;
            x = c+ h2/Number(2);
            while (x<d)
            {
                arguments[0] = x;
                integral += (h2*sign2*map_functions[function_name].evaluate(arguments));
                check_nan_flag();
            x += h2;
            }
            y += h1;

        }
        integral*=(sign1*h1);
        return integral;
    }
    Number integrate2d_line(std::string function_name, Number a, Number b, std::vector<std::string>aux_arguments)
    {
        Number sign(1); Number temp;
        if(b<a)
        {
            temp = b;
            b = a;
            a = temp;
            sign = Number(-1);
        }
        Number num_divisions(1000);
        //h is the width of the subinterval
        Number h((b-a)/num_divisions);
        //t starts at the midpoint of the first subinterval (a, a+h)
        Number t = a + h/Number(2) ;
        Number integral(0.0);
        std::vector<Number> arguments(3);
        std::vector<Number> parameter(1);
        std::string function_name_1 = aux_arguments[0];
        std::string function_name_2 = aux_arguments[1];
        while(t<b)
        {
            parameter[0] = t;
            arguments[0] = map_functions[function_name_1].evaluate(parameter);
            arguments[1] = map_functions[function_name_2].evaluate(parameter);
            integral += map_functions[function_name].evaluate(arguments) * std_functions::hypot(differentiate(function_name_1, t),differentiate(function_name_2, t));
            check_nan_flag();
            t += h;
        }
        integral *= (sign*h);
        return integral;
    }
    Number integrate3d_line(std::string function_name, Number a, Number b, std::vector<std::string>aux_arguments)
    {
        Number sign(1); Number temp;
        if(b<a)
        {
            temp = b;
            b = a;
            a = temp;
            sign = Number(-1);
        }
        Number num_divisions(1000);
        //h is the width of the subinterval
        Number h((b-a)/num_divisions);
        //t starts at the midpoint of the first subinterval (a, a+h)
        Number t = a + h/Number(2) ;
        Number integral(0.0);
        std::vector<Number> arguments(3);
        std::vector<Number> parameter(1);
        std::string function_name_1 = aux_arguments[0];
        std::string function_name_2 = aux_arguments[1];
        std::string function_name_3 = aux_arguments[2];
        while(t<b)
        {
            parameter[0] = t;
            arguments[0] = map_functions[function_name_1].evaluate(parameter);
            arguments[1] = map_functions[function_name_2].evaluate(parameter);
            arguments[2] = map_functions[function_name_3].evaluate(parameter);
            integral += map_functions[function_name].evaluate(arguments) *std_functions::hypot(std_functions::hypot(differentiate(function_name_1, t),differentiate(function_name_2, t)),differentiate(function_name_3, t));
            check_nan_flag();
            t += h;
        }
        integral *= (sign*h);
        return integral;
    }
    Number integrate3d_cub(std::string function_name, Number a, Number b, Number c, Number d, Number e, Number f)
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
        if(f<e)
        {
            temp = e;
            e = f;
            f = temp;
            sign=sign*Number(-1);
        }
        Number num_divisions(100);
        Number h1 ((b-a)/num_divisions);
        Number h2 ((d-c)/num_divisions);
        Number h3 ((f-e)/num_divisions);
        Number y, z;
        Number integral(0.0);
        std::vector<Number> arguments(3);
        Number x(a + h1/Number(2));
        while(x<b)
        {
            arguments[0]=x;
            y = c+ h2/Number(2);
            while (y<d)
            {
                arguments[1] = y;
                z = e + h3/Number(2);
                while (z<f)
                {
                    arguments[2] = z;
                    integral += map_functions[function_name].evaluate(arguments);
                    check_nan_flag();
            z += h3;
                }
                y += h2;
            }
            x += h1;
        }
        integral*=(sign*h1*h2*h3);
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
        check_nan_flag();
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
        Number allowed_error (0.00001);
        Number slope (differentiate(function_name,x));
        check_nan_flag();
        while(func_val > allowed_error || func_val < - allowed_error)
        {
            //we better our approximation by finding where the tangent at x
            //intersects the x-axis
            if(slope < allowed_error && slope > -allowed_error)
                x = x + allowed_error;
            //std::cout<<x<<std::endl;
            x = x - func_val/slope;
            slope = differentiate(function_name,x);
            arguments[0] = x;
            func_val = map_functions[function_name].evaluate(arguments);
            check_nan_flag();
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
        check_nan_flag();
        // if b is the root, return it
        if (func_b==0)
            return b;
        //if function values of a and b are of the same sign, bisection method won't work
        //therefore print error message and return 42 to tell parser not to print anything
        if(func_a*func_b>0)
        {
            std::cout<<INPUT_ERROR<<std::endl;
            suppress_zero = true;
            return Number(0.0);
        }
        //storing function value of midpoint of a and b
        arguments[0] = (a+b)/2;
        Number func_mp (map_functions[function_name].evaluate(arguments));
        check_nan_flag();
        //defining an approximate infinitesimal value to use as the epsilon about 0
        Number allowed_error (0.001); //This much variance of func_mp from 0 is treated as negligible
        Number discont_error (0.0000001); //If the interval is this narrow and root hasn't been found implies it's discontinuous
        while(func_mp > allowed_error || func_mp < -allowed_error)
        {
            //if function values of mp and a are of opposite signs, change limits to a and mp, since the root must lie between them
            if((b-a)<discont_error)
            {
                std::cout<<DISCONT_ERROR<<std::endl;
                suppress_zero = true;
                return Number(0.0);
            }
            if((func_mp< Number(0.0) && func_a>Number(0.0))||(func_mp > Number(0.0) && func_a < Number(0.0)))
            {
                b = (a+b)/2;
                arguments[0] = (a+b)/2;
                func_mp = map_functions[function_name].evaluate(arguments);
                check_nan_flag();
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
    INTEGRATE.routine_help ="help/integrate.txt";
    map_routines[INTEGRATE.routine_name] = INTEGRATE;


    Routine INTEGRATE_RM;
    INTEGRATE_RM.routine_name = "integrate.rm";
    INTEGRATE_RM.num_arguments = 2;
    INTEGRATE_RM.routine_help ="help/integrate.rm.txt";
    map_routines[INTEGRATE_RM.routine_name] = INTEGRATE_RM;

    Routine INTEGRATE_RM_N;
    INTEGRATE_RM_N.routine_name = "integrate.rm_n";
    INTEGRATE_RM_N.num_arguments = 3;
    INTEGRATE_RM_N.routine_help ="help/integrate.rm_n.txt";
    map_routines[INTEGRATE_RM_N.routine_name] = INTEGRATE_RM_N;

    Routine INTEGRATE_RT;
    INTEGRATE_RT.routine_name = "integrate.rt";
    INTEGRATE_RT.num_arguments = 2;
    INTEGRATE_RT.routine_help ="help/integrate.rt.txt";
    map_routines[INTEGRATE_RT.routine_name] = INTEGRATE_RT;

    Routine INTEGRATE_MC;
    INTEGRATE_MC.routine_name = "integrate.mc";
    INTEGRATE_MC.num_arguments = 2;
    INTEGRATE_MC.routine_help ="help/integrate.mc.txt";
    map_routines[INTEGRATE_MC.routine_name] = INTEGRATE_MC;

    Routine INTEGRATE2D_LINE;
    INTEGRATE2D_LINE.routine_name = "integrate2d.line";
    INTEGRATE2D_LINE.num_arguments = 2;
    INTEGRATE2D_LINE.routine_help = "help/integrate2d.line.txt";
    map_routines[INTEGRATE2D_LINE.routine_name] = INTEGRATE2D_LINE;

    Routine INTEGRATE2D_RECT;
    INTEGRATE2D_RECT.routine_name = "integrate2d.rect";
    INTEGRATE2D_RECT.num_arguments = 4;
    INTEGRATE2D_RECT.routine_help = "help/integrate2d.rect.txt";
    map_routines[INTEGRATE2D_RECT.routine_name] = INTEGRATE2D_RECT;

    Routine INTEGRATE2D_TYPE1;
    INTEGRATE2D_TYPE1.routine_name = "integrate2d.type1";
    INTEGRATE2D_TYPE1.num_arguments = 2;
    INTEGRATE2D_TYPE1.routine_help = "help/integrate2d.type1.txt";
    map_routines[INTEGRATE2D_TYPE1.routine_name] = INTEGRATE2D_TYPE1;

    Routine INTEGRATE2D_TYPE2;
    INTEGRATE2D_TYPE2.routine_name = "integrate2d.type2";
    INTEGRATE2D_TYPE2.num_arguments = 2;
    INTEGRATE2D_TYPE2.routine_help = "help/integrate2d.type2.txt";
    map_routines[INTEGRATE2D_TYPE2.routine_name] = INTEGRATE2D_TYPE2;

    Routine INTEGRATE3D_LINE;
    INTEGRATE3D_LINE.routine_name = "integrate3d.line";
    INTEGRATE3D_LINE.num_arguments = 2;
    INTEGRATE3D_LINE.routine_help = "help/integrate3d.line.txt";
    map_routines[INTEGRATE3D_LINE.routine_name] = INTEGRATE3D_LINE;

    Routine INTEGRATE3D_CUB;
    INTEGRATE3D_CUB.routine_name = "integrate3d.cub";
    INTEGRATE3D_CUB.num_arguments = 6;
    INTEGRATE3D_CUB.routine_help = "help/integrate3d.cub.txt";
    Routine DIFFERENTIATE;
    DIFFERENTIATE.routine_name = "differentiate";
    DIFFERENTIATE.num_arguments = 1;
    map_routines[DIFFERENTIATE.routine_name] = DIFFERENTIATE;

    Routine NEWTON;
    NEWTON.routine_name = "solve.n";
    NEWTON.num_arguments = 1;
    NEWTON.routine_help ="help/solve.n.txt";
    map_routines[NEWTON.routine_name] = NEWTON;

    Routine BISECTION;
    BISECTION.routine_name = "solve.b";
    BISECTION.num_arguments = 2;
    BISECTION.routine_help ="help/solve.b.txt";
    map_routines[BISECTION.routine_name] = BISECTION;

    return;
}

void def_ndarrays()
{
    ndArray array_test;
    array_test.array_name = "the_42";
    array_test.dim = 1;
    map_ndarrays[array_test.array_name] = array_test;

    return;

}
