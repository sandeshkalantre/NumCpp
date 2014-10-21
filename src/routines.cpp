#include <iostream>
#include <string>
namespace std

vector <double> arguments[1];
arguments[0]=x;

double diff1 (string function_name, double a)
{
double h = 0.000001;
//evaluate the value of the function just at the given point
double fx= map functions [function_name].evaluate(a);
//evaluate the value of the function just to the right of the given point
double fx1 = map functions [function_name].evaluate(a+h);
double slope = (fx1 - fx )/ h;
return slope;

}


double diff2 (string function_name, double a)
{
   //define h as tends to 0 somehow
   double h = 0.0000001;
   //value of the func. just to x's right
   double fx2 = map functions [function_name].evaluate(x+h) ;
   //value of the func. just to x's left
   double fx1 = map functions [function_name].evaluate(x-h) ;
    //slope to the right of x
   double slope2 = ( fx2 - fx ) / h ;
    //slope to the left of x
   double slope1 = ( fx - fx1 ) / h ;
   //eventually taking the avergae gives you the same result as the central approx. method
   //central approx: f'(x) = (f(x+h) - f(x-h)) / 2h;
   double avslope = ( slope1 + slope2 ) / 2 ;
   return avslope;

}

double diff3 (string function_name, double a)
{
//we will evaluate the function at 5 points around a.
double h = 0.0000001;
//value of function at distance 2h to the left of the point
double fx1= map functions [function_name].evaluate(x-(2*h)) ;
//value of function at distance h to the left of the point
double fx2 = map functions [function_name].evaluate(x-h) ;
//value of function at distance 2h to the right of the point
double fx3 = map functions [function_name].evaluate(x+h) ;
//value of function at distance h to the right of the point
double fx4 = map functions [function_name].evaluate(x+(2*h)) ;
double ans = ( fx1 - (8*fx2) + (8*fx3) - fx4 )/ (12*h) ;
return ans;

}
