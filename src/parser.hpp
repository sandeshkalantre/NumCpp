/*
This file contains the parsing classes Token and Parser
as well as the data structure classes Number and ndArray
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

#define BASE 10
#define RND_MODE MPFR_RNDZ
#define DEFAULT_PRECISION 64

typedef mpfr_t cppdouble;

class Token;
class Parser;
class Function;
class Routine;
class Number;
class ndArray;

//the extern maps which will store our variables and functions and routines
extern std::map<std::string, Function> map_functions;
extern std::map<std::string, Number> map_variables;
extern std::map<std::string, Routine> map_routines;
extern std::map<std::string, ndArray> map_ndarrays;

//the global bools which are used when parsing fails
extern bool suppress_zero;
extern bool suppress_eval;

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
    public:
        //stores the rpn in function_rpn given the rpn as the argument
        void store_rpn(std::queue<Token> rpn);

        //this has to written differently than eval_rpn as d_arguments are the variables
        //and not the variables in map_variables
        Number evaluate(std::vector<Number> d_arguments);

        //this function is called by evaluate if the function being evaluated is
        //a standard function as stored in bool standard
        Number std_evaluate(std::vector<Number> d_arguments);
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
        Number evaluate(std::string function_name, std::vector<Number> arguments);
        void help();
};

class Number
{
    //since mpfr_t is a c styled pointer to an array it seems proper
    //to keep value public instead of private for simplicity of code
    public:
    cppdouble value;

    public:
    Number();
    Number(cppdouble _value);
    Number(double _value);
    Number(int _value);

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
    bool operator==(const double num2);
};

class ndArray
{
    public:
        ndArray();

        //name of the array
        std::string array_name;

        //number of dimensions
        int dim;

        //vector of the size of dimensions
        std::vector<int> dim_size;

        //the vector stored as a map with the key as the index of the element
        //store in a vector
        std::map<std::vector<int>,Number> array;

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
};

#endif
