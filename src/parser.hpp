#ifndef PARSER_HPP
#define PARSER_HPP

#include "misc.h"
#include "functions.h"
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




class Token;
class Parser;
class Function;

extern std::map<std::string, Function> map_functions;
extern std::map<std::string, double> map_variables;


class Token
{
    //allow Parser and Function to have access to Token_type and other variables
    friend class Parser;
    friend class Function;

    //enumerated types which define important token characteristics such as token_type
    //operator_id,precedence and associativity.
    private:
        enum TOKEN_TYPE
        {
            NIL,
            NUMBER,
            VARIABLE,
            OPERATOR,
            FUNCTION,
            LPAREN,
            RPAREN,
            COMMA,
            SEMICOLON,
            //this type is used when the string "define" is used on the output
            //the allows the parser to know that we are defining something
            DEFINE,
            //this sign denotes define/assignment of the variable/function
            EQUAL_SIGN,
            UNKNOWN
        };

        enum OPERATOR_ID
        {
            NOTANOPERATOR,
            PLUS,MINUS,
            MULTIPLY,DIVIDE,MODULUS,
            POWER,
            FACTORIAL
        };

        enum OPERATOR_PRECEDENCE
        {
            LEVEL0,
            //+ and -
            LEVEL1,
            //*,/,%
            LEVEL2,
            //^,!
            LEVEL3
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

    //functions
    public:
        //will check if the statement is a definition or a call to a function in the library
        //if it is a definition it will create a Function class for the function in the Map_functions
        //and use math_parse to store it's rpn
        //if it is a routine/function call to the library, it will call the function and output the result
        void parse(std::string expr);

        //converts the given expression to rpn
        void math_parse(std::string expr, std::string::iterator it_expr);

        //evaluates the rpn
        double eval_rpn(std::queue<Token> expr_rpn);
};


class Function
{

    public:
        //name of the function
        std::string function_name;
        //number of the arguments of the function
        //default number of arguments is zero
        int num_arguments;
    public:
        //the vector of arguments
        std::vector<std::string> s_arguments;
        //the unordered map of argument names and their values
        std::map<std::string, double> map_arguments;
        //the rpn form of the definition of the function
        std::queue<Token> function_rpn;
        //stores whether the function is a standard function;
        bool standard;
    public:
        Function();
        void store_rpn(std::queue<Token> rpn);
        //this has to written differently than eval_rpn as d_arguments are the variables
        //and not the variables in map_variables
        double evaluate(std::vector<double> d_arguments);
	double std_evaluate(std::vector<double> d_arguments);
};









#endif






