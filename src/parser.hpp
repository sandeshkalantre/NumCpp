/*
This file contains the parsing classes Token and Parser
as well as the data structure classes Number,ndArray,
Complex,Complex_array.

It also contains the classes Function,Routine and sim_eqn.
*/

#ifndef PARSER_HPP
#define PARSER_HPP

#include "misc.h"
#include "functions.h"
#include "routines.h"
#include "variables.h"
#include "mpfr.h"
#include <cmath>
#include <gmp.h>
#include <string>
#include <queue>
#include <map>
#include <vector>
#include <stack>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <cstdlib>
#include <fstream>

//base of the number system
#define BASE 10
//rouding mode for mpfr
#define RND_MODE MPFR_RNDZ
//default precison
#define DEFAULT_PRECISION 64
//default dim and ndArray
#define DEFAULT_DIM 3

typedef mpfr_t cppdouble;

class Token;
class Parser;
class Function;
class Routine;
class Number;
class ndArray;
class Complex;
class Complex_array;
class sim_eqn;

//the extern maps which will store our variables and functions and routines
extern std::map<std::string, Function> map_functions;
extern std::map<std::string, Number> map_variables;
extern std::map<std::string, Routine> map_routines;
extern std::map<std::string, ndArray> map_ndarrays;

//the global bools which are used when parsing fails
extern bool suppress_zero;
extern bool suppress_eval;

//prints the help present in filename
void help(std::string filename);

class Token
{
    //allow Parser and Function to have access to token_type and other variables
    friend class Parser;
    friend class Function;
    friend class Routines;
    friend class ndArray;

    //enumerated types which define important token characteristics such as token_type
    //operator_id,precedence and associativity.
    private:
        enum TOKEN_TYPE
        {
            //DEFAULT token_type when the object is created
            //different from UNKNOWN
            NIL,
            NUMBER,
            NDARRAY,
            OPERATOR,
            VARIABLE,
            FUNCTION,
            ROUTINE,
            LPAREN,
            RPAREN,
            SQ_LPAREN,
            SQ_RPAREN,
            COMMA,
            SEMICOLON,
            COLON,
            EQUAL_SIGN,
            //KEYWORDS

            //this type is used when the string "define" is used on the output
            //the allows the parser to know that we are defining something
            DEFINE,
            //keyword to denote showing of an RPN
            SHOW_RPN,
            //keyword to define a linspace array
            LINSPACE,
            ZEROS,
            ONES,
            //keyword for reading and writing to a file
            WRITE,
            READ,
            //keyword for function evaluation on an array
            EVALUATE,
            //help keyword
            HELP,
            //fft keyword
            FFT,
            //solve for simultaneous equations
            SOLVE,
            UNKNOWN
        };

        enum OPERATOR_ID
        {
            NOTANOPERATOR,
            PLUS,
            MINUS,
            UNARY_MINUS,
            E,//denotes scientific notation
            MULTIPLY,
            DIVIDE,
            MODULUS,
            POWER,
            FACTORIAL
        };

        enum OPERATOR_PRECEDENCE
        {
            LEVEL0,
            //+ and -
            LEVEL1,
            //*,/,%,e
            LEVEL2,
            //^,!
            LEVEL3,
            //unary minus
            LEVEL4
        };

        enum OPERATOR_ASSOCIATIVITY
        {
            NONE,
            LEFT,
            RIGHT
        };

    //data
    public:
        //string storing the token
        std::string token;

        //stores the token type
        TOKEN_TYPE token_type;

        //stores the operator id in case of an operator
        //NOTANOPERATOR = 0 otherwise
        OPERATOR_ID operator_id;

        //stores the operator precedence level in case of an operator
        //LEVEL0 = 0 otherwise
        OPERATOR_PRECEDENCE operator_precedence;

        //stores the operator associativity
        //NONE = 0 otherwise
        OPERATOR_ASSOCIATIVITY operator_associativity;

    //constructor for the Token class
    public:
        Token();

    //functions
    public:
        enum OPERATOR_ID get_operator_id(const char c);
        enum OPERATOR_PRECEDENCE get_operator_precedence(const char c);
        enum OPERATOR_ASSOCIATIVITY get_operator_associativity(const char c);

        //stores the token attributes and returns the iterator to the next element in the string expr
        std::string::iterator get_token(std::string expr, std::string::iterator it_expr);
};

class Parser
{
    //constructor for the class Parser
    public:
        Parser();

    public:
        //the parsed expression in RPN form as a vector of tokens
        std::queue<Token> expr_rpn;
        //the parsed form of the slice stored as a queue of integers
        std::queue<int> slice;

    //functions
    public:
        /*
        The Parse function does the following jobs:
        1)checks if the statement is a definition or a call to a function in the library
        if it is a definition it will create a Function class for the function in the Map_functions
        and use math_parse to store it's rpn
        2)if it is a routine/function call to the library, it will call the function and output the result
        3)check if the expression is a definition of ndarray and store it accordingly
        */
        void parse(std::string expr);

        //converts the given expression to rpn
        void math_parse(std::string expr, std::string::iterator it_expr);

        //used to parse slice definitions
        std::queue<int> slice_parse(ndArray array,std::string expr,std::string::iterator it_expr);

        //evaluates the rpn
        Number eval_rpn(std::queue<Token> expr_rpn);

        //evaluates the rpn to return a number stack
        //used in defintions
        std::stack<Number> eval_rpn_num_stack(std::queue<Token> expr_rpn);

};

class Function
{
    friend class Routine;
    friend class Parser;

    //data
    public:
        //name of the function
        std::string function_name;

        //number of the arguments of the function
        //default number of arguments is zero
        int num_arguments;

        //stores whether the function is a standard function;
        bool standard;

    private:
        //the vector of arguments
        std::vector<std::string> s_arguments;

        //the ordered map of argument names and their values
        std::map<std::string, Number> map_arguments;

        //the rpn form of the definition of the function
        std::queue<Token> function_rpn;

    //functions
    public:
        //the constructor for the class Functions
        Function();

        //stores the rpn in function_rpn given the rpn as the argument
        void store_rpn(std::queue<Token> rpn);

        //this has to written differently than eval_rpn as d_arguments are the variables
        //and not the variables in map_variables
        Number evaluate(std::vector<Number> d_arguments);

        //this function is called by evaluate if the function being evaluated is
        //a standard function as stored in bool standard
        Number std_evaluate(std::vector<Number> d_arguments);

        //help for functions
        static std::string functions_help;
};

class Routine
{
    friend class Function;
    public:
        //name of routine
        std::string routine_name;
        //number of arguments excluding the function_name
        int num_arguments;
        //string which stores the help for the routine
        std::string routine_help;

    public:
        //evaluates the routine given the function name and the vector of arguments
        //aux arguments
        Number evaluate(std::string function_name, std::vector<Number> arguments,std::vector<std::string> aux_arguments);

        //list of routines
        static std::string routines_help;
};

class Number
{
    //since mpfr_t is a c styled pointer to an array it seems proper
    //to keep value public instead of private for simplicity of code
    public:
    cppdouble value;

    public:
    //constructors for initialization
    Number();
    Number(cppdouble _value);
    Number(double _value);
    Number(int _value);

    //overloaded operators for the Number
    Number operator+(const Number num2);
    Number operator-(const Number num2);
    Number operator*(const Number num2);
    Number operator/(const Number num2);
    Number operator%(const Number num2);
    Number operator^(const Number num2);
    Number operator+=(const Number num2);
    Number operator-=(const Number num2);
    Number operator*=(const Number num2);
    Number operator/=(const Number num2);
    Number operator=(const double num2);
    Number operator++();
    Number operator--();
    Number operator-();
    bool operator>(const Number num2);
    bool operator<(const Number num2);
    bool operator>=(const Number num2);
    bool operator<=(const Number num2);
    bool operator==(const Number num2);
    bool operator!=(const Number num2);
    bool operator==(const double num2);

    //help for the constants stored in map_variables
    static std::string constants_help;
};

class ndArray
{
    friend class Complex_array;
    friend class sim_eqn;
    friend class Parser;
    private:
        //the vector stored as a map with the key as the index of the element
        //store in a vector
        std::map<std::vector<int>,Number> array;

    public:
        //constructors
        ndArray();

        ndArray(const Complex_array c_array);

        //name of the array
        std::string array_name;

        //number of dimensions
        int dim;

        //vector of the size of dimensions
        std::vector<int> dim_size;

        //store value at the given index
        void store_value(std::vector<int> index,Number value);

        //returns value at the given index
        Number return_value(std::vector<int> index);

        //prints the complete array
        void show();

        //parses the explicit definition of the array
        void array_def_parse(std::string expr,std::string::iterator it_expr);

        //defines a linspace array consisting of num_points doubles front start to end
        //END IS NOT INCLUSIVE
        void define_linspace(Number start,Number end,int num_points);

        //prints the slice
        void show_slice(std::queue<int> slice);

        //store in a file
        void write_to_file(const std::string out_filename);

        //read from a file
        void read_from_file(const std::string in_filename);

        //eval a function on all elements of an array and store the result
        //in output_array
        void evaluate(std::string function_name,std::string output_array_name);

        //conversion functions from ndArrays to arrays and vice versa
        void set2d(Number** A);
        void set1d(Number* A);
        void get1d(Number* A,unsigned long size);
};

//class of complex numbers
class Complex
{
    private:
        //internal representation i_
        Number i_real;
        Number i_img;
    public:
        //const j:sqrt(-1)
        static const Complex j;

        //constructors
        Complex(): i_real(0.0),i_img(0.0){};
        Complex(Number _real,Number _img): i_real(_real),i_img(_img){};
        //real number
        Complex(Number _real):i_real(_real),i_img(0.0){};

        //assignment for a real number
        Complex operator=(Number _real);

        //return the real and imaginary parts respectively
        Number real();
        Number img();

        //conjugate
        //does not change the original complex number
        Complex conjugate();

        //the norm of the complex number
        Number norm();

        //the operators overloaded for members of the Complex class
        Complex operator+(Complex c_num2);
        Complex operator-(Complex c_num2);
        Complex operator*(Complex c_num2);
        Complex operator/(Complex c_num2);
        Complex operator+=(Complex c_num2);
        Complex operator-=(Complex c_num2);
        Complex operator*=(Complex c_num2);
        Complex operator/=(Complex c_num2);
        bool operator==(Complex c_num2);
        bool operator!=(Complex c_num2);
        Complex operator-();
};

class Complex_array
{
    private:
        unsigned long i_size;
        Complex* c_array;
    public:
        unsigned long size();
        void store_value(const Complex c_num,unsigned long index);
        Complex return_value(unsigned long index);

        //constructors
        Complex_array();
        Complex_array(ndArray array);
        //arrays of complex numbers with zeros
        Complex_array(unsigned long _size);

        //subscripting operator
        Complex operator[](unsigned long index);

        //scale the array by a real number
        void scale(Number scale);

        //the fft function
        void forward_fft(Complex_array c_array_fft);

    private:
        void rearrange();
        void fft();
};

class sim_eqn
{
    private:
        //coefficient matrix,the rhs and the array for the solution
        Number **A,*B,*X;
        long n;
    public:
        //constructors
        sim_eqn();
        sim_eqn(unsigned long m);

        //the input and output functions for the arrays
        Number* get();
        void set(ndArray _A,ndArray _B);
        void solve(long start);

        ~sim_eqn();
};
#endif
