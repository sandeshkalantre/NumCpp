#ifndef PARSER_HPP
#define PARSER_HPP

#include "misc.h"
#include "functions.h"
#include "routines.h"
#include <cmath>
#include <string>
#include <queue>
//#include <unordered_map>
#include <map>
#include <vector>
#include <stack>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <iostream>
//for atof function
//stof requires C++11
//DECIDE WHETHER TO USE stof or atof
#include <cstdlib>
#include <fstream>

class Token;
class Parser;
class Function;
class Routine;
class ndArray;

//the extern maps which will store our variables and functions and routines
extern std::map<std::string, Function> map_functions;
extern std::map<std::string, double> map_variables;
extern std::map<std::string, Routine> map_routines;
extern std::map<std::string, ndArray> map_ndarrays;


class Token
{
    //allow Parser and Function to have access to Token_type and other variables
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
            WRITE,
            READ,
            EVALUATE,
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
        double eval_rpn(std::queue<Token> expr_rpn);

        //evaluates the rpn to return a number stack
        //used in defintions
        std::stack<double> eval_rpn_num_stack(std::queue<Token> expr_rpn);

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
        std::map<std::string, double> map_arguments;

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
        double evaluate(std::vector<double> d_arguments);

        //this function is called by evaluate if the function being evaluated is
        //a standard function as stored in bool standard
        double std_evaluate(std::vector<double> d_arguments);
};

class Routine
{
    friend class Function;
    public:
        //name of routine
        std::string routine_name;
        //number of arguments excluding the function_name
        int num_arguments;

    public:
        //evaluates the routine given the function name and the vector of arguments
        double evaluate(std::string function_name, std::vector<double> arguments);
};

class ndArray
{
    public:
        ndArray();

        //name of the vector
        std::string array_name;

        //number of dimensions
        int dim;

        //vector of the size of dimensions
        std::vector<int> dim_size;

        //the vector stored as a map with the key as the index of the element
        //store in a vector
        std::map<std::vector<int>,double> array;

        //store value at the given index
        void store_value(std::vector<int> index,double value);

        //returns value at the given index
        double return_value(std::vector<int> index);

        //prints the complete array
        void show();

        //parses the explicit definition of the array
        void array_def_parse(std::string expr,std::string::iterator it_expr);

        //defines a linspace array consisting of num_points doubles front start to end
        //END IS NOT INCLUSIVE
        void define_linspace(double start,double end,int num_points);

        //array with all elements as zeros
        bool is_zeros;
        void define_zeros();

        //array with all elements as ones
        bool is_ones;
        void define_ones();

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






