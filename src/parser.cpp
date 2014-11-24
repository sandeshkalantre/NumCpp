#include "parser.hpp"
#include "functions.h"
#include "routines.h"
#include "misc.h"
#include "errors.h"
#include "complex.h"
#include "mpfr.h"
#include <gmp.h>

std::string Number::constants_help = "help/constants.txt";
std::string Function::functions_help = "help/functions.txt";
std::string Routine::routines_help = "help/routines.txt";

void help(std::string filename)
{
    char c;
    FILE* fp;
    fp = fopen(filename.c_str(),"r");
    try
    {
        if(fp == NULL)
        {
            throw FILE_OPEN_FAILED;
        }
        while((c = getc(fp)) != EOF)
        {
            putchar(c);
        }
        fclose(fp);
    }
    catch(const char* str)
    {
        std::cout<<str<<std::endl;
    }
}

//IMPLEMENTATION OF THE TOKEN CLASS
//Token class constructor
Token::Token()
{
    token_type = NIL;
    operator_id = NOTANOPERATOR;
    operator_precedence = LEVEL0;
    operator_associativity = NONE;
}

//returns the id of the operator according to the enum OPERATOR_ID
enum Token::OPERATOR_ID Token::get_operator_id(const char c)
{
    if(c == '+')
    {
        return PLUS;
    }
    if(c == '-')
    {
        //UNARY_MINUS is determined while parsing in Parser::math_parse
        //by default all minus signs denote subtraction
        return MINUS;
    }
    if(c == '*')
    {
        return MULTIPLY;
    }
    if(c == '/')
    {
        return DIVIDE;
    }
    if(c == '%')
    {
        return MODULUS;
    }
    if(c == 'E')
    {
        return E;
    }
    if(c == '^')
    {
        return POWER;
    }
    if(c == '!')
    {
        return FACTORIAL;
    }
    return NOTANOPERATOR;
}

enum Token::OPERATOR_PRECEDENCE Token::get_operator_precedence(const char c)
{
    if(c == '+' || c == '-')
    {
        return LEVEL1;
    }
    if(c == '*' || c == '/' || c == '%' || c == 'e' || c == 'E')
    {
        return LEVEL2;
    }
    if(c == '^' || c == '!')
    {
        return LEVEL3;
    }
    return LEVEL0;
}

enum Token::OPERATOR_ASSOCIATIVITY Token::get_operator_associativity(const char c)
{
    if(c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '!' || c == 'e' || c == 'E')
    {
        return LEFT;
    }
    if(c == '^')
    {
        return RIGHT;
    }
    return NONE;
}

std::string::iterator Token::get_token(std::string expr,std::string::iterator it_expr)
{
    //clear the token string
    if(!token.empty())
    {
        token.clear();
    }

    //skip over whitespaces
    while(std::isspace(*it_expr))
    {
        it_expr++;
    }

    //checks for a semicolon
    if(is_semicolon(*it_expr))
    {
        token_type = SEMICOLON;
        token += *it_expr;
        it_expr++;
        return it_expr;
    }

    //checks for a colon
    if(is_colon(*it_expr))
    {
        token_type = COLON;
        token += *it_expr;
        it_expr++;
        return it_expr;
    }

    //check for an equal('=') sign
    if(is_equal_sign(*it_expr))
    {
        token_type = EQUAL_SIGN;
        token += *it_expr;
        it_expr++;
        return it_expr;
    }

    //checks if the char is a function argument separator comma ','
    if(is_comma(*it_expr))
    {
        token_type = COMMA;
        token += *it_expr;
        it_expr++;
        return it_expr;
    }

    //checks whether the characters are a part of a number
    //and stores then in the token
    if(std::isdigit(*it_expr)) //|| *it_expr == '.')
    //'.' part was commented out
    //otherwise it treats strings beginning with . as a number
    //the part was there to handle numbers such as .123 but
    //we have decided that such numbers will not be allowed
    //ALL NUMBERS START WITH A DIGIT
    {
        token_type = NUMBER;
        while(std::isdigit(*it_expr) || *it_expr == '.')
        {
            token += *it_expr;
            it_expr++;
        }
        return it_expr;
    }

    //checks whether the token is an operator among +,-,*,/,%,^,!,E
    if(is_operator(*it_expr))
    {
        token_type = OPERATOR;
        operator_id = get_operator_id(*it_expr);
        operator_precedence = get_operator_precedence(*it_expr);
        operator_associativity = get_operator_associativity(*it_expr);
        token += *it_expr;
        it_expr++;
        return it_expr;
    }

    //checks whether the token is a left parentheses
    if(is_lparen(*it_expr))
    {
        token_type = LPAREN;
        token += *it_expr;
        it_expr++;
        return it_expr;
    }

    //checks whether the token is a right parentheses
    if(is_rparen(*it_expr))
    {
        token_type = RPAREN;
        token += *it_expr;
        it_expr++;
        return it_expr;
    }

    //checks whether the token is a square left parentheses
    if(is_sq_lparen(*it_expr))
    {
        token_type = SQ_LPAREN;
        token += *it_expr;
        it_expr++;
        return it_expr;
    }

    //checks whether the token is a square right parentheses
    if(is_sq_rparen(*it_expr))
    {
        token_type = SQ_RPAREN;
        token += *it_expr;
        it_expr++;
        return it_expr;
    }

     //checks whether the token is a name either of a variable or a function or a routine
    //or is a keyword
    if(std::isalpha(*it_expr) || *it_expr == '_' || *it_expr == '.')
    {
        while(std::isalnum(*it_expr) || *it_expr == '_' || *it_expr == '.')
        {
            token += *it_expr;
            it_expr++;
        }

        //checks whether the token is a define keyword
        //which the parser uses so as to know that the user is defining
        //a variable or function
        if(is_define(token))
        {
            token_type = DEFINE;
            return it_expr;
        }

        //checks whether the token is show rpn keyword
        if(is_showrpn(token))
        {
            token_type =SHOW_RPN;
            return it_expr;
        }

        //checks whether the token is the name of a routine
        if(is_routine(token))
        {
            token_type = ROUTINE;
            return it_expr;
        }
        //checks whether the token is the keyword linspace
        if(is_linspace(token))
        {
            token_type = LINSPACE;
            return it_expr;
        }

        //check whether the token is the keyword ones
        if(is_ones(token))
        {
            token_type = ONES;
            return it_expr;
        }

        //check whether the token is the keyword zeros
        if(is_zeros(token))
        {
            token_type = ZEROS;
            return it_expr;
        }

        //checks whether the token is the keyword read
        if(is_read(token))
        {
            token_type = READ;
            return it_expr;
        }

        //checks whether the token is the keyword write
        if(is_write(token))
        {
            token_type = WRITE;
            return it_expr;
        }

         //checks whether the token is the keyword evaluate
        if(is_evaluate(token))
        {
            token_type = EVALUATE;
            return it_expr;
        }

        //checks whether the token is the keyword help
        if(is_help(token))
        {
            token_type = HELP;
            return it_expr;
        }

        //checks whether the token is the keyword fft
        if(is_fft(token))
        {
            token_type = FFT;
            return it_expr;
        }

        //checks whether the token is the keyword solve
        if(is_solve(token))
        {
            token_type = SOLVE;
            return it_expr;
        }

        //checks whether the token is the keyword sci
        if(is_sci(token))
        {
            token_type = SCI;
            return it_expr;
        }

        //check whether this is a variable or a function
        //function has a left parentheses

        //skip whitespace
        while(std::isspace(*it_expr))
        {
            it_expr++;
        }

        //check if it is a function as a function will have  a left parentheses
        if(is_lparen(*it_expr))
        {
            token_type = FUNCTION;
        }
        else if(is_sq_lparen(*it_expr))
        {
            token_type = NDARRAY;
        }
        else
        {
            token_type = VARIABLE;
        }
        return it_expr;
    }

    //if no token type was matched
    token_type = UNKNOWN;
    std::cout<<*it_expr<<" : "<<UNKNOWN_TOKEN<<std::endl;
    suppress_eval = true;
    return it_expr;
}

//IMPLEMENTATION OF THE CLASS PARSER
//constructor for the class Parser
Parser::Parser()
{
}

void Parser::parse(std::string expr)
{
    //set the supress eval flag
    suppress_eval = false;
    std::string::iterator it_expr = expr.begin();
    Token token;
    it_expr = token.get_token(expr, it_expr);

    //return if the token is UNKNOWN
    if(token.token_type == Token::UNKNOWN)
    {
        return;
    }

    //DEFINITIONS
    //store a variable or a function or an ndArray
    if(token.token_type == Token::DEFINE)
    {
        //once it is define we check whether we are defining
        //a variable or function again by using the Token class
        //which can tell us whether the next token is a variable or a function
        //as function will have an LPAREN after it's name
        it_expr = token.get_token(expr, it_expr);
        if(token.token_type == Token::VARIABLE)
        {
            //store the variable name which will go in the map_varibales
            std::string var_name = token.token;

            //stores the variable value which goes into the map_varibales
            Number value;

            it_expr = token.get_token(expr, it_expr);
            if(token.token_type == Token::EQUAL_SIGN)
            {
                Parser math_parser;
                //on the RHS of equal sign is an expression which must be stored as the value of the variable
                math_parser.math_parse(expr, it_expr);
                //the rpn is stored in math_parser.expr_prn
                //use eval_rpn method to generate the value of the rpn and store as the
                //value of the variable
                value = math_parser.eval_rpn(math_parser.expr_rpn);
            }
            else
            {
                std::cout<<MISSING_EQUAL_SIGN<<std::endl;
                return;
            }

            //store the values in the map
            map_variables[var_name] = value;
            return;
        }

        //store a function
        if(token.token_type == Token::FUNCTION)
        {
            //will store the function in map_functions
            Function function;
            function.function_name = token.token;

            //ignore the LPAREN
            it_expr = token.get_token(expr,it_expr);
            if(token.token_type != Token::LPAREN)
            {
                std::cout<<MISSING_LEFT_PARENTHESES<<std::endl;
                return;
            }

            while(token.token_type != Token::RPAREN)
            {
                //token = Token();
                it_expr = token.get_token(expr, it_expr);
                if(token.token_type == Token::VARIABLE)
                {
                    function.num_arguments++;
                    function.s_arguments.push_back(token.token);
                    function.map_arguments[token.token];
                }
                else if(token.token_type != Token::RPAREN && token.token_type != Token::COMMA)
                {
                    std::cout<<token.token<<" is not a valid function argument."<<std::endl;
                }

                //if RPAREN is never encountered
                if(token.token_type == Token::SEMICOLON)
                {
                    std::cout<<MISSING_RIGHT_PARENTHESES<<std::endl;
                    return;
                }
            }
            //arguments have ended and the RPAREN has been been handled
            //This gets the EQUAL_SIGN
            it_expr = token.get_token(expr, it_expr);
            if(token.token_type == Token::EQUAL_SIGN)
            {
                Parser math_parser;
                //on the RHS of equal sign is an expression which must be stored as the value of the function
                math_parser.math_parse(expr, it_expr);
                //the rpn is stored in math_parser.expr_prn
                function.function_rpn = math_parser.expr_rpn;
            }
            else
            {
                std::cout<<MISSING_EQUAL_SIGN<<std::endl;
            }

            //stores the function in map_functions in map_functions
            map_functions[function.function_name] = function;
            return;
        }

        //define an ndArray
        if(token.token_type == Token::NDARRAY)
        {
            //will store the ndarray in map_ndarrays
            ndArray array;
            array.array_name = token.token;
            array.dim = 0;

            //ignore the LPAREN
            it_expr = token.get_token(expr,it_expr);
            if(token.token_type != Token::SQ_LPAREN)
            {
                std::cout<<MISSING_LEFT_PARENTHESES<<std::endl;
                return;
            }

            while(token.token_type != Token::SQ_RPAREN)
            {
                it_expr = token.get_token(expr, it_expr);
                if(token.token_type == Token::NUMBER)
                {
                    double number = atof(token.token.c_str());
                    if(number == abs(floor(number)) && number > 0)
                    {
                        array.dim++;
                        array.dim_size.push_back(atoi(token.token.c_str()));
                    }
                    else
                    {
                        std::cout<<number<<" is not a valid array dimension"<<std::endl;
                        return;
                    }
                }
                else if(token.token_type != Token::SQ_RPAREN && token.token_type != Token::COMMA)
                {
                    std::cout<<token.token<<" is not a valid array dimension."<<std::endl;
                }

                //if SQ_RPAREN is never found
                if(token.token_type == Token::SEMICOLON)
                {
                    std::cout<<MISSING_RIGHT_PARENTHESES<<std::endl;
                    return;
                }
            }
            //std::reverse(array.dim_size.begin(),array.dim_size.end());

            //store the vector

            //the next token is a equal
            it_expr = token.get_token(expr,it_expr);
            if(token.token_type != Token::EQUAL_SIGN)
            {
                std::cout<<MISSING_EQUAL_SIGN<<std::endl;
            }

            std::string::iterator it_expr_temp = it_expr;
            it_expr = token.get_token(expr,it_expr);
            //checks whether the array is defined using a linspace
            if(token.token_type == Token::LINSPACE)
            {
                if(array.dim != 1)
                {
                    std::cout<<"Linspace is a 1D array."<<std::endl;
                    return;
                }
                Number start,end;
                int num_points = array.dim_size[0];
                //next token is an LPAREN
                std::string::iterator it_expr_lparen = it_expr;
                it_expr = token.get_token(expr,it_expr);
                if(token.token_type == Token::LPAREN)
                {
                    try
                    {
                        math_parse(expr,it_expr_lparen);
                        if(suppress_eval)
                        {
                            return;
                        }
                        std::stack<Number> number_stack = eval_rpn_num_stack(expr_rpn);
                        if(number_stack.size() != 2)
                        {
                            throw INVALID_EXPRESSION;
                        }
                        end = number_stack.top();
                        number_stack.pop();

                        start = number_stack.top();
                        number_stack.pop();
                    }
                    catch(const char *str)
                    {
                        std::cout<<str<<std::endl;
                        return;
                    }
                }
                else
                {
                    std::cout<<MISSING_LEFT_PARENTHESES<<std::endl;
                }

                array.define_linspace(start,end,num_points);
                map_ndarrays[array.array_name] = array;
                return;
            }

            else if(token.token_type == Token::EVALUATE)
            {
                math_parse(expr,it_expr);
                //expr_rpn will have two components
                //the function name and the array_name
                std::string in_array_name = expr_rpn.front().token;
                expr_rpn.pop();
                std::string function_name = expr_rpn.front().token;

                //temp is used to allow A = evaluate(A,f) behaviour
                ndArray temp_array = map_ndarrays[in_array_name];
                temp_array.array_name = "_";
                map_ndarrays[temp_array.array_name] = temp_array;
                map_ndarrays[array.array_name] = array;
                map_ndarrays[temp_array.array_name].evaluate(function_name,array.array_name);
                return;


            }
            //if the rhs is itself the name of an array/ndArray token
            //then the contents of rhs are copied to the lhs
            else if(token.token_type == Token::NDARRAY)
            {
                try
                {
                    if(map_ndarrays.count(token.token) <= 0)
                    {
                        throw token.token;
                    }
                    map_ndarrays[array.array_name] = map_ndarrays[token.token];
                    return;
                }
                catch(std::string str)
                {
                    std::cout<<token.token<<" "<<ARRAY_NOT_DEFINED<<std::endl;
                    return;
                }
            }


            else
            {
                it_expr = it_expr_temp;
            }

            //the array is defined element-wise
            try
            {
                array.array_def_parse(expr,it_expr);
            }
            catch(const char *str)
            {
                std::cout<<IMPROPER_EXPRESSION<<std::endl;
                return;
            }
            //store the array in the map
            map_ndarrays[array.array_name] = array;
            return;
        }
    }

    //SHOW_RPN keyword
    if(token.token_type == Token::SHOW_RPN)
    {
        //if the argument is not a function then it is an expression
        math_parse(expr,it_expr);
        if(suppress_eval)
        {
            return;
        }
        std::queue<Token> expr_rpn_temp = expr_rpn;
        std::vector<std::string> function_names;
        while(!expr_rpn_temp.empty())
        {
            std::cout<<expr_rpn_temp.front().token;
            if(expr_rpn_temp.front().token_type == Token::FUNCTION)
            {
                function_names.push_back(expr_rpn_temp.front().token);
            }
            expr_rpn_temp.pop();
        }
        std::cout<<std::endl;

        //if the argument is a function name,then will show
        //the stored rpn of the function
        while(!function_names.empty())
        {
            //all functions have been defined as math_parse already checks this
            std::cout<<function_names.back()<<" : rpn"<<std::endl;
            //copy the rpn queue stored in the function
            if(map_functions[function_names.back()].standard)
            {
                std::cout<<"RPN of standard functions is not stored"<<std::endl;
                function_names.pop_back();
                continue;
            }
            std::queue<Token> function_rpn_temp = map_functions[function_names.back()].function_rpn;
            while(!function_rpn_temp.empty())
            {
                std::cout<<function_rpn_temp.front().token;
                function_rpn_temp.pop();
            }
            std::cout<<std::endl;
            function_names.pop_back();
        }
        return;
    }

    //read keyword
    if(token.token_type == Token::READ)
    {
        math_parse(expr,it_expr);
        if(suppress_eval)
        {
            return;
        }
        //expr_rpn will have two components
        //the array name and the file name
        std::string array_name = expr_rpn.front().token;
        expr_rpn.pop();
        std::string filename = expr_rpn.front().token;

        try
        {
            map_ndarrays[array_name].read_from_file(filename);
        }
        catch(const char *str)
        {
            std::cout<<str<<std::endl;
            return;
        }
        return;

    }
    //write keyword
    if(token.token_type == Token::WRITE)
    {
        math_parse(expr,it_expr);
        if(suppress_eval)
        {
            return;
        }
        //expr_rpn will have two components
        //the array name and the file name
        std::string array_name = expr_rpn.front().token;
        expr_rpn.pop();
        std::string filename = expr_rpn.front().token;
        expr_rpn.pop();
        if(!expr_rpn.empty()  && expr_rpn.front().token == "csv")
        {
            ndArray array;
            array.array_name = array_name;
            try
            {
                if(map_ndarrays.count(array_name) <= 0)
                {
                    throw ARRAY_NOT_DEFINED;
                }
                map_ndarrays[array_name].write_to_file_csv(filename);
            }
            catch(const char *str)
            {
                std::cout<<str<<std::endl;
                return;
            }
            return;
        }


        ndArray array;
        array.array_name = array_name;
        try
        {
            if(map_ndarrays.count(array_name) <= 0)
            {
                throw ARRAY_NOT_DEFINED;
            }
            map_ndarrays[array_name].write_to_file(filename);
        }
        catch(const char *str)
        {
            std::cout<<str<<std::endl;
            return;
        }
        return;
    }

    //evaluate keyword for functions on arrays
    if(token.token_type == Token::EVALUATE)
    {
        math_parse(expr,it_expr);
        if(suppress_eval)
        {
            return;
        }
        //expr_rpn will have two components
        //the function name and the array_name
        if(map_ndarrays.count(expr_rpn.front().token) <= 0)
        {
            std::cout<<ARRAY_NOT_DEFINED<<std::endl;
            return;
        }
        std::string array_name = expr_rpn.front().token;
        expr_rpn.pop();
        if(map_functions.count(expr_rpn.front().token) <= 0)
        {
            std::cout<<FUNCTION_NOT_DEFINED<<std::endl;
            return;
        }
        std::string function_name = expr_rpn.front().token;

        ndArray temp_array = map_ndarrays[array_name];
        temp_array.array_name = "_";
        map_ndarrays[temp_array.array_name] = temp_array;
        map_ndarrays[array_name].evaluate(function_name,temp_array.array_name);
        map_ndarrays[temp_array.array_name].show();

        return;
    }

    //help token
    if(token.token_type == Token::HELP)
    {
        math_parse(expr,it_expr);
        if(suppress_eval)
        {
            return;
        }
        //list of functions
        if(expr_rpn.front().token.compare("functions") == 0)
        {
            help(Function::functions_help);
            return;
        }
        //list of constants
        if(expr_rpn.front().token.compare("constants") == 0)
        {
            help(Number::constants_help);
            return;
        }
        //list of routines
        if(expr_rpn.front().token.compare("routines") == 0)
        {
            help(Routine::routines_help);
            return;
        }
        //help for routines
        while(!expr_rpn.empty())
        {
            if(expr_rpn.front().token_type != Token::ROUTINE)
            {
                std::cout<<expr_rpn.front().token<<" : "<<NOT_A_ROUTINE<<std::endl;
                return;
            }
            help(map_routines[expr_rpn.front().token].routine_help);
            expr_rpn.pop();
        }
        return;

    }

    if(token.token_type == Token::FFT)
    {
        math_parse(expr,it_expr);
        if(suppress_eval)
        {
            return;
        }
        //the token must be an ndArray of dim 2 and size n*2
        if(expr_rpn.front().token_type == Token::NDARRAY)
        {
            try
            {
                //check whether the array size is a power of 2
                if(map_ndarrays[expr_rpn.front().token].dim_size[0] & (map_ndarrays[expr_rpn.front().token].dim_size[0] - 1))
                {
                    throw DIM_MISMATCH;
                }
                ndArray temp_array;// = map_ndarrays[expr_rpn.front().token];
                temp_array.dim = 2;
                temp_array.dim_size = map_ndarrays[expr_rpn.front().token].dim_size;
                Complex_array complex_array(map_ndarrays[expr_rpn.front().token]);
                if(!expr_rpn.empty() && expr_rpn.front().token == "inv")
                {
                    complex_array.inverse_fft();
                }
                else
                {
                    complex_array.forward_fft();
                }
                temp_array.get_ndarray(complex_array);
                temp_array.dim_size = map_ndarrays[expr_rpn.front().token].dim_size;
                temp_array.array_name = "_";
                map_ndarrays[temp_array.array_name] = temp_array;
                //temp_array.show();
                map_ndarrays[temp_array.array_name].show();
                return;
            }
            catch(const char *str)
            {
                std::cout<<str<<std::endl;
                return;
            }
        }
    }
    //solve token for simultaneous equations
    if(token.token_type == Token::SOLVE)
    {
        math_parse(expr,it_expr);
        if(suppress_eval)
        {
            return;
        }
        //expr_rpn will have two components
        //the two array_names
        if(map_ndarrays.count(expr_rpn.front().token) <= 0)
        {
            std::cout<<ARRAY_NOT_DEFINED<<std::endl;
            return;
        }
        std::string array_name_A = expr_rpn.front().token;
        expr_rpn.pop();
        if(map_ndarrays.count(expr_rpn.front().token) <= 0)
        {
            std::cout<<ARRAY_NOT_DEFINED<<std::endl;
            return;
        }
        std::string array_name_B = expr_rpn.front().token;
        expr_rpn.pop();
        ndArray _A = map_ndarrays[array_name_A];
        ndArray _B = map_ndarrays[array_name_B];
        try
        {
            if(_A.dim != 2 || _A.dim_size[0] != _A.dim_size[1] || _B.dim != 1 || _A.dim_size[0] != _B.dim_size[0])
            {
                throw DIM_MISMATCH;
            }
        }
        catch(const char* str)
        {
            std::cout<<str<<std::endl;
            return;
        }
        unsigned long n = _A.dim_size[0];
        sim_eqn solver(n);
        solver.set(_A,_B);
        solver.solve(0);

        ndArray temp_array = _B;
        temp_array.get1d(solver.get(),n);
        temp_array.array_name = "_";
        map_ndarrays[temp_array.array_name] = temp_array;
        std::cout<<"Solution vector: \n";
        map_ndarrays[temp_array.array_name].show();

        return;
    }

    //evaluation in scientific notation
    if(token.token_type == Token::SCI)
    {
        math_parse(expr, it_expr);
        if(suppress_eval)
        {
            return;
        }
        try
        {
            Number result;
            result = eval_rpn(expr_rpn);
            map_variables["_"] = result;

            if(!suppress_zero)
            mpfr_printf("%.15Rg \n",result.value);
        }
        catch(const char *str)
        {
            std::cout<<str<<std::endl;
        }
        return;

    }

    //EVALUATIONS
    //variable/function/routine evaluation
    it_expr = expr.begin();
    math_parse(expr, it_expr);

    //DEBUG:PRINT THE RPN AFTER PARSING
    /*
    while(!expr_rpn.empty())
    {
        std::cout<<expr_rpn.front().token;
        expr_rpn.pop();
    }
    std::cout<<std::endl;
    */
    try
    {
        Number result;
        result = eval_rpn(expr_rpn);
        map_variables["_"] = result;

        if(!suppress_zero)
        mpfr_printf("%.15Rf \n",result.value);
    }
    catch(const char *str)
    {
        std::cout<<str<<std::endl;
    }
    return;
}

void Parser::math_parse(std::string expr,std::string::iterator it_expr)
{
    //implementation of Dijsktra's shunting yard algorithm for parsing the input

    suppress_zero = false;
    //the stack of operators and functions
    std::stack<Token> operator_stack;
    //this is needed to decide whether minus denotes subtraction
    //or is a unary minus
    Token previous_token;
    Token token = Token();

	while(true)
    {
        previous_token = token;
        it_expr = token.get_token(expr,it_expr);

		if(token.token_type == Token::NUMBER || token.token_type == Token::VARIABLE)
        {
            expr_rpn.push(token);
            continue;
        }
        if(token.token_type == Token::NDARRAY)
        {
            if(map_ndarrays.count(token.token) <= 0)
            {
                std::cout<<token.token<<" : "<<ARRAY_NOT_DEFINED<<std::endl;
                suppress_zero = true;
                suppress_eval = true;
                return;
            }
            expr_rpn.push(token);
            //parse the slice
            try
            {
                slice = slice_parse(map_ndarrays[token.token],expr,it_expr);
            }
            catch(const char *str)
            {
                return;
            }
            continue;

        }

        if(token.token_type == Token::FUNCTION)
        {
            operator_stack.push(token);
            continue;
        }

        if(token.token_type == Token::ROUTINE)
        {
            operator_stack.push(token);
            continue;
        }

        if(token.token_type == Token::COMMA)
        {
            //THROW SOME ERRORS HERE IF NO LPAREN IS ENCOUNTERED
            while(!operator_stack.empty() && operator_stack.top().token_type != Token::LPAREN)
            {
                expr_rpn.push(operator_stack.top());
                operator_stack.pop();
            }
            continue;
        }
        if(token.token_type == Token::OPERATOR)
		{
            if(token.operator_id == Token::MINUS)
            {
                //handling the unary minus
                if(previous_token.token_type == Token::NUMBER
                || previous_token.token_type == Token::VARIABLE || previous_token.token_type == Token::FUNCTION
                || previous_token.token_type == Token::ROUTINE)
                {
                    //do nothing as the token by default is stored as a binary minus/subtract
                }
                else
                {
                    token.operator_associativity = Token::RIGHT;
                    token.operator_precedence = Token::LEVEL4;
                    token.operator_id = Token::UNARY_MINUS;
                }
            }

            while(!operator_stack.empty() && operator_stack.top().token_type == Token::OPERATOR)
            {
                if(token.operator_associativity == Token::LEFT && token.operator_precedence <= operator_stack.top().operator_precedence)
                {
                    expr_rpn.push(operator_stack.top());
                    operator_stack.pop();
                }
                else if(token.operator_precedence < operator_stack.top().operator_precedence)
                {
                    expr_rpn.push(operator_stack.top());
                    operator_stack.pop();
                }
                else
                {
                    break;
                }
            }
            operator_stack.push(token);
            continue;
        }

        if(token.token_type == Token::LPAREN)
        {
            operator_stack.push(token);
            continue;
        }

        if(token.token_type == Token::RPAREN)
        {
            if(operator_stack.empty())
            {
                std::cout<<MISSING_PARENTHESES<<std::endl;
                suppress_zero = true;
                suppress_eval = true;
                return;
            }
            while(operator_stack.top().token_type != Token::LPAREN)
            {
                expr_rpn.push(operator_stack.top());
                operator_stack.pop();
                if(operator_stack.empty())
                {
                    //THROW SOME ERRORS HERE IF NO LPAREN IS ENCOUNTERED
                    std::cout<<MISSING_PARENTHESES<<std::endl;
                    suppress_zero = true;
                    suppress_eval = true;
                    return;
                }
            }

            //pop the LPAREN
            operator_stack.pop();

            //if the top of the operator stack is a function
            //then this RPAREN marks the end of the arguments of the function/routine
            //the function/routine so must be pushed to the output queue
            if(!operator_stack.empty() && (operator_stack.top().token_type == Token::FUNCTION || operator_stack.top().token_type == Token::ROUTINE))
            {
                expr_rpn.push(operator_stack.top());
                operator_stack.pop();
            }
            continue;
        }

        if(token.token_type == Token::SEMICOLON)
        {
            break;
        }

        if(token.token_type == Token::UNKNOWN)
        {
            suppress_eval = true;
            return;
        }
    }

    //if there are still operators on the stack,pop them to the output
    while(!operator_stack.empty())
    {
        if(operator_stack.top().token_type == Token::LPAREN || operator_stack.top().token_type == Token::RPAREN)
        {
            std::cout<<MISSING_PARENTHESES<<std::endl;
            suppress_eval = true;
            suppress_zero = true;
            return;
        }
        expr_rpn.push(operator_stack.top());
        operator_stack.pop();
    }
    return;
}

//will evaluate the rpn tokens passed to in queue<token> expr_rpn
Number Parser::eval_rpn(std::queue<Token> expr_rpn)
{
    if(suppress_eval)
    {
        suppress_zero = true;
        return Number(0.0);
    }
    std::stack<Number> number_stack;
    //this is used as argument to routines
    std::string routine_function_name;
    //auxillary arguments to a routine
    std::vector<std::string> routine_aux_arguments;

    while(!expr_rpn.empty())
    {
        if(expr_rpn.front().token_type == Token::NUMBER)
        {
            Number result;
            //mpfr function is used for conversion from string to Number
            mpfr_set_str(result.value,expr_rpn.front().token.c_str(),BASE,RND_MODE);
            number_stack.push(result);
            expr_rpn.pop();
            continue;
        }
        if(expr_rpn.front().token_type == Token::VARIABLE)
        {
            //check whether the variable has been defined
            if(map_variables.count(expr_rpn.front().token) <= 0)
            {
                std::cout<<expr_rpn.front().token<<" : "<<VARIABLE_NOT_DEFINED<<std::endl;
                suppress_zero = true;
                return Number(0.0);
            }

            number_stack.push(map_variables[expr_rpn.front().token]);
            expr_rpn.pop();
            continue;
        }
        if(expr_rpn.front().token_type == Token::NDARRAY)
        {
            //check whether the array has been defined
            if(map_ndarrays.count(expr_rpn.front().token) <= 0)
            {
                std::cout<<expr_rpn.front().token<<" : "<<ARRAY_NOT_DEFINED<<std::endl;
                suppress_zero = true;
                return Number(0.0);
            }
            if(slice.empty())
            {
                map_ndarrays[expr_rpn.front().token].show();
                suppress_zero = true;
                return Number(0.0);
            }
            else
            {
                map_ndarrays[expr_rpn.front().token].show_slice(slice);
                expr_rpn.pop();
                suppress_zero = true;
                return Number(0.0);
            }
            continue;
        }
        if(expr_rpn.front().token_type == Token::OPERATOR)
        {
            if(expr_rpn.front().operator_id != Token::FACTORIAL && expr_rpn.front().operator_id != Token::UNARY_MINUS)
            {
                if(number_stack.size() < 2)
                {
                    throw INVALID_EXPRESSION;
                }
                //store the top two elements as components of a vector
                std::vector<Number>top_two (2);

                top_two[1] = number_stack.top();
                number_stack.pop();
                top_two[0] = number_stack.top();
                number_stack.pop();

                if(expr_rpn.front().operator_id == Token::PLUS)
                {
                    number_stack.push(map_functions["ADD"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::MINUS)
                {
                    number_stack.push(map_functions["SUBTRACT"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }

                if(expr_rpn.front().operator_id == Token::MULTIPLY)
                {
                    number_stack.push(map_functions["MULTIPLY"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::DIVIDE)
                {
                    number_stack.push(map_functions["DIVIDE"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::MODULUS)
                {
                    number_stack.push(map_functions["MODULUS"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::E)
                {
                    number_stack.push(map_functions["E"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::POWER)
                {
                    number_stack.push(map_functions["POWER"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }

            }
            //unary operators
            else
            {
                if(number_stack.size() < 1)
                {
                    throw INVALID_EXPRESSION;
                }
                std::vector<Number> top (1);
                top[0] = number_stack.top();
                number_stack.pop();
                if(expr_rpn.front().operator_id == Token::FACTORIAL)
                {
                    number_stack.push(map_functions["FACTORIAL"].evaluate(top));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::UNARY_MINUS)
                {
                    number_stack.push(map_functions["UNARY_MINUS"].evaluate(top));
                    expr_rpn.pop();
                    continue;
                }
            }
        }
        if(expr_rpn.front().token_type == Token::FUNCTION)
        {
			//ERROR HANDLING TO ENSURE THAT THE FUNCTION EXISTS ON THE Map_functions
            if(map_functions.count(expr_rpn.front().token) <= 0)
            {
                std::cout<<expr_rpn.front().token<<" : "<<FUNCTION_NOT_DEFINED<<std::endl;
                suppress_zero = true;
                return Number(0.0);
            }
			std::queue<Token> temp_expr_rpn;
			temp_expr_rpn = expr_rpn;

			while(temp_expr_rpn.front().token_type == Token::FUNCTION)
			{
                routine_aux_arguments.push_back(temp_expr_rpn.front().token);
                temp_expr_rpn.pop();
            }
			if(temp_expr_rpn.front().token_type == Token::ROUTINE)
			{
                //the last argument is the routine_function name
				routine_function_name = routine_aux_arguments.back();
				routine_aux_arguments.pop_back();
				expr_rpn = temp_expr_rpn;
				continue;
			}
			else
			{
                routine_aux_arguments.clear();
			}

		    //stores the poped arguments from the number_stack
            std::vector<Number> arguments;

            //WRITE ERROR HANDLING IF THERE ARE NOT ENOUGH ARGUMENTS ON THE number_stack
            if((int)number_stack.size() < map_functions[expr_rpn.front().token].num_arguments)
            {
                throw INVALID_EXPRESSION;
            }
            for(int i = 0; i < map_functions[expr_rpn.front().token].num_arguments; i++)
            {
                arguments.push_back(number_stack.top());
                number_stack.pop();
            }
            //the arguments have to be reversed as they are stored in rpn in reverse order
            std::reverse(arguments.begin(),arguments.end());

            number_stack.push(map_functions[expr_rpn.front().token].evaluate(arguments));

            expr_rpn.pop();
            continue;
        }
        if(expr_rpn.front().token_type == Token::ROUTINE)
        {
			std::vector<Number> arguments;
			if((int)number_stack.size() < map_routines[expr_rpn.front().token].num_arguments)
			{
                throw INVALID_EXPRESSION;
			}
            for(int i = 0; i < map_routines[expr_rpn.front().token].num_arguments; i++)
            {
				arguments.push_back(number_stack.top());
                number_stack.pop();
            }
            //the arguments have to be reversed as they are stored in rpn in reverse order
            std::reverse(arguments.begin(),arguments.end());

            number_stack.push(map_routines[expr_rpn.front().token].evaluate(routine_function_name,arguments,routine_aux_arguments));
            expr_rpn.pop();
            continue;
        }

        if(expr_rpn.front().token_type == Token::UNKNOWN)
        {
            return Number(0.0);
        }
    }

    if(number_stack.size() == 1)
    {
        if(mpfr_nanflag_p() != 0 || mpfr_divby0_p() !=0)
        {
            mpfr_clear_nanflag();
            mpfr_clear_divby0();
            throw DOMAIN_ERROR;
        }
        return number_stack.top();
    }
    else
    {
        suppress_zero = true;
        throw INVALID_EXPRESSION;
    }
    //return 0 by default
    return Number(0.0);
}

//will evaluate the rpn tokens passed to in queue<token> expr_rpn
std::stack<Number> Parser::eval_rpn_num_stack(std::queue<Token> expr_rpn)
{
    std::stack<Number> number_stack;
    //this is used as argument to routines
    std::string routine_function_name;
    //auxillary arguments to a routine
    std::vector<std::string> routine_aux_arguments;


    while(!expr_rpn.empty())
    {
        if(expr_rpn.front().token_type == Token::NUMBER)
        {
            Number result;
            mpfr_set_str(result.value,expr_rpn.front().token.c_str(),BASE,RND_MODE);
            number_stack.push(result);
            expr_rpn.pop();
            continue;
        }
        if(expr_rpn.front().token_type == Token::VARIABLE)
        {
            number_stack.push(map_variables[expr_rpn.front().token]);
            expr_rpn.pop();
            continue;
        }
        if(expr_rpn.front().token_type == Token::NDARRAY)
        {
            if(slice.empty())
            {
                map_ndarrays[expr_rpn.front().token].show();
            }
            else
            {
                map_ndarrays[expr_rpn.front().token].show_slice(slice);
                expr_rpn.pop();
            }
            continue;
        }
        if(expr_rpn.front().token_type == Token::OPERATOR)
        {
            if(expr_rpn.front().operator_id != Token::FACTORIAL && expr_rpn.front().operator_id != Token::UNARY_MINUS)
            {
                //ERROR HANDLING TO CHECK THAT THERE ATLEAST TWO ELEMENTS ON THE STACK
                if(number_stack.size() < 2)
                {
                    throw INVALID_EXPRESSION;
                }

                //store the top two elements as components of a vector
                std::vector<Number>top_two (2);

                top_two[1] = number_stack.top();
                number_stack.pop();
                top_two[0] = number_stack.top();
                number_stack.pop();

                if(expr_rpn.front().operator_id == Token::PLUS)
                {
                    number_stack.push(map_functions["ADD"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::MINUS)
                {
                    number_stack.push(map_functions["SUBTRACT"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::MULTIPLY)
                {
                    number_stack.push(map_functions["MULTIPLY"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::DIVIDE)
                {
                    number_stack.push(map_functions["DIVIDE"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::MODULUS)
                {
                    number_stack.push(map_functions["MODULUS"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::E)
                {
                    number_stack.push(map_functions["E"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::POWER)
                {
                    number_stack.push(map_functions["POWER"].evaluate(top_two));
                    expr_rpn.pop();
                    continue;
                }

            }
            //unary operators
            else
            {
                if(number_stack.size() < 1)
                {
                    throw INVALID_EXPRESSION;
                }
                std::vector<Number> top (1);
                top[0] = number_stack.top();
                number_stack.pop();
                if(expr_rpn.front().operator_id == Token::FACTORIAL)
                {
                    number_stack.push(map_functions["FACTORIAL"].evaluate(top));
                    expr_rpn.pop();
                    continue;
                }
                if(expr_rpn.front().operator_id == Token::UNARY_MINUS)
                {
                    number_stack.push(map_functions["UNARY_MINUS"].evaluate(top));
                    expr_rpn.pop();
                    continue;
                }

            }

        }

        if(expr_rpn.front().token_type == Token::FUNCTION)
        {

			//ERROR HANDLING TO ENSURE THAT THE FUNCTION EXISTS ON THE Map_functions
            if(map_functions.count(expr_rpn.front().token) <= 0)
            {
                std::cout<<expr_rpn.front().token<<" : "<<FUNCTION_NOT_DEFINED<<std::endl;
                suppress_zero = true;
                return number_stack;
            }

			std::queue<Token> temp_expr_rpn;
			temp_expr_rpn = expr_rpn;
            while(temp_expr_rpn.front().token_type == Token::FUNCTION)
			{
                routine_aux_arguments.push_back(temp_expr_rpn.front().token);
                temp_expr_rpn.pop();
            }
			if(temp_expr_rpn.front().token_type == Token::ROUTINE)
			{
                //the last argument is the function name
				routine_function_name = routine_aux_arguments.back();
				routine_aux_arguments.pop_back();
				expr_rpn = temp_expr_rpn;
				continue;
			}
			else
			{
                routine_aux_arguments.clear();
			}

		    //stores the poped arguments from the number_stack
            std::vector<Number> arguments;

            //WRITE ERROR HANDLING IF THERE ARE NOT ENOUGH ARGUMENTS ON THE number_stack
            if((int)number_stack.size() < map_functions[expr_rpn.front().token].num_arguments)
            {
                throw INVALID_EXPRESSION;
            }

            for(int i = 0; i < map_functions[expr_rpn.front().token].num_arguments; i++)
            {
                arguments.push_back(number_stack.top());
                number_stack.pop();
            }
            //the arguments have to be reversed as they are stored in rpn in reverse order
            std::reverse(arguments.begin(),arguments.end());

            number_stack.push(map_functions[expr_rpn.front().token].evaluate(arguments));

            expr_rpn.pop();
            continue;
        }

        if(expr_rpn.front().token_type == Token::ROUTINE)
        {
			std::vector<Number> arguments;
			if((int)number_stack.size() < map_routines[expr_rpn.front().token].num_arguments)
			{
                throw INVALID_EXPRESSION;
			}
            for(int i = 0; i < map_routines[expr_rpn.front().token].num_arguments; i++)
            {
				arguments.push_back(number_stack.top());
                number_stack.pop();
            }
            //the arguments have to be reversed as they are stored in rpn in reverse order
            std::reverse(arguments.begin(),arguments.end());

            number_stack.push(map_routines[expr_rpn.front().token].evaluate(routine_function_name,arguments,routine_aux_arguments));
            expr_rpn.pop();
            continue;
        }
    }
    return number_stack;
}
//the slice is parsed using this function
std::queue<int> Parser::slice_parse(ndArray array,std::string expr,std::string::iterator it_expr)
{
    std::queue<int> slice;
    Token token;
    Token previous_token;
    int current_dim = -1;

    while(true)
    {
        it_expr = token.get_token(expr,it_expr);
        if(token.token_type == Token::COLON)
        {
            current_dim++;
            if(previous_token.token_type == Token::SQ_LPAREN)
            {
                slice.push(0);
            }
            if(previous_token.token_type == Token::COMMA)
            {
                slice.push(0);
            }
            previous_token = token;
            continue;
        }
        if(token.token_type == Token::COMMA)
        {
            if(previous_token.token_type == Token::COLON)
            {
                slice.push(array.dim_size[current_dim]);
            }
            previous_token = token;
            continue;
        }
        if(token.token_type == Token::OPERATOR && token.operator_id == Token::MINUS)
        {
            previous_token = token;
            continue;
        }

        if(token.token_type == Token::NUMBER)
        {
            int num =  atoi(token.token.c_str());
            //negative slices are also handled!
            if(previous_token.token_type == Token::OPERATOR && token.operator_id == Token::MINUS)
            {
                num *= -1;
            }

            if(num < 0 && num > (-1*array.dim_size[current_dim + 1]))
            {
                num += array.dim_size[current_dim + 1];
            }

            slice.push(num);
            previous_token = token;
            continue;
        }
        if(token.token_type == Token::SQ_RPAREN)
        {
            if(previous_token.token_type == Token::COLON)
            {
                slice.push(array.dim_size[current_dim]);
            }
            break;
        }
        previous_token = token;
        if(token.token_type == Token::UNKNOWN)
        {
            throw UNKNOWN_TOKEN;
            break;
        }
    }
    return slice;
}

//IMPLEMENTATION OF THE CLASS FUNCTION
Function::Function()
{
    function_name = "NIL";
    num_arguments = 0;
    standard = false;
}
void Function::store_rpn(std::queue<Token> rpn)
{
    function_rpn = rpn;
    return;
}
Number Function::std_evaluate(std::vector<Number> d_arguments)
{
    if(function_name == "ADD")
    {
        return std_functions::add(d_arguments[0], d_arguments[1]);
    }
    if(function_name == "SUBTRACT")
    {
        return std_functions::subtract(d_arguments[0], d_arguments[1]);
    }
    if(function_name == "UNARY_MINUS")
    {
        return std_functions::unary_minus(d_arguments[0]);
    }
    if(function_name == "MULTIPLY")
    {
        return std_functions::multiply(d_arguments[0], d_arguments[1]);
    }
    if(function_name == "DIVIDE")
    {
        return std_functions::divide(d_arguments[0], d_arguments[1]);
    }
    if(function_name == "E")
    {
        return std_functions::scientific(d_arguments[0], d_arguments[1]);
    }
    if(function_name == "FACTORIAL")
    {
        return std_functions::factorial(d_arguments[0]);
    }
    if(function_name == "POWER")
    {
        return std_functions::power(d_arguments[0], d_arguments[1]);
    }
    if(function_name == "MODULUS")
    {
        return std_functions::modulus1(d_arguments[0], d_arguments[1]);
    }
    if(function_name == "sin")
    {
        return std_functions::sin(d_arguments[0]);
    }
    if(function_name == "cos")
    {
        return std_functions::cos(d_arguments[0]);
    }
    if(function_name == "tan")
    {
        return std_functions::tan(d_arguments[0]);
    }
    if(function_name == "sec")
    {
        return std_functions::sec(d_arguments[0]);
    }
    if(function_name == "csc")
    {
        return std_functions::csc(d_arguments[0]);
    }
    if(function_name == "cot")
    {
        return std_functions::cot(d_arguments[0]);
    }
    if(function_name == "asin")
    {
        return std_functions::asin(d_arguments[0]);
    }
    if(function_name == "acos")
    {
        return std_functions::acos(d_arguments[0]);
    }
    if(function_name == "atan")
    {
        return std_functions::atan(d_arguments[0]);
    }
    if(function_name == "sinh")
    {
        return std_functions::sinh(d_arguments[0]);
    }
    if(function_name == "cosh")
    {
        return std_functions::cosh(d_arguments[0]);
    }
    if(function_name == "tanh")
    {
        return std_functions::tanh(d_arguments[0]);
    }
    if(function_name == "sech")
    {
        return std_functions::sech(d_arguments[0]);
    }
    if(function_name == "csch")
    {
        return std_functions::csch(d_arguments[0]);
    }
    if(function_name == "coth")
    {
        return std_functions::coth(d_arguments[0]);
    }
    if(function_name == "asinh")
    {
        return std_functions::asinh(d_arguments[0]);
    }
    if(function_name == "acosh")
    {
        return std_functions::acosh(d_arguments[0]);
    }
    if(function_name == "atanh")
    {
        return std_functions::atanh(d_arguments[0]);
    }
    if(function_name == "atan2")
    {
        return std_functions::atan2(d_arguments[0],d_arguments[1]);
    }
    if(function_name == "log1p")
    {
        return std_functions::log1p(d_arguments[0]);
    }
    if(function_name == "expm1")
    {
        return std_functions::expm1(d_arguments[0]);
    }
    if(function_name == "eint")
    {
        return std_functions::eint(d_arguments[0]);
    }
    if(function_name == "li2")
    {
        return std_functions::li2(d_arguments[0]);
    }
    if(function_name == "gamma")
    {
        return std_functions::gamma(d_arguments[0]);
    }
    if(function_name == "lngamma")
    {
        return std_functions::lngamma(d_arguments[0]);
    }
    if(function_name == "digamma")
    {
        return std_functions::digamma(d_arguments[0]);
    }
    if(function_name == "zeta")
    {
        return std_functions::zeta(d_arguments[0]);
    }
    if(function_name == "erf")
    {
        return std_functions::erf(d_arguments[0]);
    }
    if(function_name == "erfc")
    {
        return std_functions::erfc(d_arguments[0]);
    }
    if(function_name == "j0")
    {
        return std_functions::j0(d_arguments[0]);
    }
    if(function_name == "j1")
    {
        return std_functions::j1(d_arguments[0]);
    }
    if(function_name == "jn")
    {
        return std_functions::jn(d_arguments[0],d_arguments[1]);
    }
    if(function_name == "y0")
    {
        return std_functions::y0(d_arguments[0]);
    }
    if(function_name == "y1")
    {
        return std_functions::y1(d_arguments[0]);
    }
    if(function_name == "yn")
    {
        return std_functions::yn(d_arguments[0],d_arguments[1]);
    }
    if(function_name == "agm")
    {
        return std_functions::agm(d_arguments[0],d_arguments[1]);
    }
    if(function_name == "hypot")
    {
        return std_functions::hypot(d_arguments[0],d_arguments[1]);
    }
    if(function_name == "ai")
    {
        return std_functions::ai(d_arguments[0]);
    }
    //by deafult it returns zero
    return Number(0.0);
}

Number Function::evaluate(std::vector<Number> d_arguments)
{
    suppress_zero = false;
    //stores the rpn so that it can be used again
    std::queue<Token> function_rpn_temp = function_rpn;

    //if the function is a standard function,then uses std_evaluate
    if(standard)
    {
        return std_evaluate(d_arguments);
    }

    //copy the d_arguments to the map_arguments
    for(int i = 0; i < num_arguments; i++)
    {
        map_arguments[s_arguments[i]] = d_arguments[i];
    }

    std::stack<Number> number_stack;

    while(!function_rpn.empty())
    {
        if(function_rpn.front().token_type == Token::NUMBER)
        {
            Number result;
            mpfr_set_str(result.value,function_rpn.front().token.c_str(),BASE,MPFR_RNDN);
            number_stack.push(result);
            function_rpn.pop();
        }
        if(function_rpn.front().token_type == Token::VARIABLE)
        {
            //preference is first as an argument then a variable
            if(map_arguments.count(function_rpn.front().token) > 0)
            {
                number_stack.push(map_arguments[function_rpn.front().token]);
                function_rpn.pop();
            }
            else if(map_variables.count(function_rpn.front().token) > 0)
            {
                number_stack.push(map_variables[function_rpn.front().token]);
                function_rpn.pop();
            }
            else
            {
                std::cout<<function_rpn.front().token<<" : "<<VARIABLE_NOT_DEFINED<<std::endl;
                suppress_zero = true;
                return Number(0.0);
            }
        }
        if(function_rpn.front().token_type == Token::OPERATOR)
        {
            //WRITE SOME ERROR HANDLING TO CHECK THAT THERE ATLEAST TWO ELEMENTS ON THE STACK
            if(function_rpn.front().operator_id != Token::FACTORIAL && function_rpn.front().operator_id != Token::UNARY_MINUS)
            {
                //store the top two elements as compenets of a vector
                if(number_stack.size() < 2)
                {
                    throw INVALID_EXPRESSION;
                }
                std::vector<Number>top_two (2);
                top_two[1] = number_stack.top();
                number_stack.pop();
                top_two[0] = number_stack.top();
                number_stack.pop();

                if(function_rpn.front().operator_id == Token::PLUS)
                {
                    number_stack.push(map_functions["ADD"].evaluate(top_two));
                    function_rpn.pop();
                    continue;
                }
                if(function_rpn.front().operator_id == Token::MINUS)
                {
                    number_stack.push(map_functions["SUBTRACT"].evaluate(top_two));
                    function_rpn.pop();
                    continue;
                }
                if(function_rpn.front().operator_id == Token::MULTIPLY)
                {
                    number_stack.push(map_functions["MULTIPLY"].evaluate(top_two));
                    function_rpn.pop();
                    continue;
                }
                if(function_rpn.front().operator_id == Token::DIVIDE)
                {
                    number_stack.push(map_functions["DIVIDE"].evaluate(top_two));
                    function_rpn.pop();
                    continue;
                }
                if(function_rpn.front().operator_id == Token::E)
                {
                    number_stack.push(map_functions["E"].evaluate(top_two));
                    function_rpn.pop();
                    continue;
                }
                if(function_rpn.front().operator_id == Token::POWER)
                {
                    number_stack.push(map_functions["POWER"].evaluate(top_two));
                    function_rpn.pop();
                    continue;
                }
            }
            else
            {
                if(number_stack.size() < 1)
                {
                    throw INVALID_EXPRESSION;
                }
                std::vector<Number> top (1);
                top[0] = number_stack.top();
                number_stack.pop();
                if(function_rpn.front().operator_id == Token::FACTORIAL)
                {
                    number_stack.push(map_functions["FACTORIAL"].evaluate(top));
                    function_rpn.pop();
                    continue;
                }
                if(function_rpn.front().operator_id == Token::UNARY_MINUS)
                {
                    number_stack.push(map_functions["UNARY_MINUS"].evaluate(top));
                    function_rpn.pop();
                    continue;
                }
            }

        }

        if(function_rpn.front().token_type == Token::FUNCTION)
        {
            //ERROR HANDLING TO ENSURE THAT THE FUNCTION EXISTS ON THE Map_functions
            if(map_functions.count(function_rpn.front().token) <= 0)
            {
                std::cout<<"Definition Error : "<<function_rpn.front().token<<FUNCTION_NOT_DEFINED<<std::endl;
                suppress_zero = true;
                return Number(0.0);
            }

            //stores the poped arguments from the number_stack
            std::vector<Number> arguments;

            //ERROR HANDLING IF THERE ARE NOT ENOUGH ARGUMENTS ON THE number_stack
            if((int)number_stack.size() < map_functions[function_rpn.front().token].num_arguments)
            {
                throw INVALID_EXPRESSION;
            }
            for(int i = 0; i < map_functions[function_rpn.front().token].num_arguments; i++)
            {
                arguments.push_back(number_stack.top());
                number_stack.pop();
            }
            number_stack.push(map_functions[function_rpn.front().token].evaluate(arguments));

            function_rpn.pop();

        }
    }

    function_rpn = function_rpn_temp;

    if(number_stack.size() == 1)
    {
        if(mpfr_nanflag_p() != 0 || mpfr_divby0_p() !=0)
        {
            mpfr_clear_nanflag();
            mpfr_clear_divby0();
            throw DOMAIN_ERROR;
        }
        return number_stack.top();
    }
    else
    {
        suppress_zero = true;
        throw INVALID_EXPRESSION;
    }
    //returns zero by default
    return Number(0.0);
}

Number Routine::evaluate(std::string function_name,std::vector<Number> arguments,std::vector<std::string> aux_arguments)
{
    if(routine_name.compare("differentiate") == 0)
    {
        return routines::differentiate(function_name,arguments[0]);
    }
    if(routine_name.compare("partial.diff2d") == 0)
    {
        if(arguments[2] == Number(0))
        {
            aux_arguments.push_back("x");
        }
        if(arguments[2] == Number(1))
        {
            aux_arguments.push_back("y");
        }
        return routines::partial_diff2d(arguments[0],arguments[1],function_name,aux_arguments);
    }
    if(routine_name.compare("partial.diff3d") == 0)
    {
        if(arguments[2] == Number(0))
        {
            aux_arguments.push_back("x");
        }
        if(arguments[2] == Number(1))
        {
            aux_arguments.push_back("y");
        }
        if(arguments[2] == Number(2))
        {
            aux_arguments.push_back("z");
        }
        else
        {
            throw ARGUMENT_ERROR;
        }
        return routines::partial_diff3d(arguments[0],arguments[1],arguments[2],function_name,aux_arguments);
    }
    if(routine_name.compare("integrate") == 0)
    {
        return routines::integrate_rm(function_name,arguments[0],arguments[1]);
    }
    if(routine_name.compare("integrate.rm") == 0)
    {
        return routines::integrate_rm(function_name,arguments[0],arguments[1]);
    }
    if(routine_name.compare("integrate.rm_n") == 0)
    {
        return routines::integrate_rm_n(function_name,arguments[0],arguments[1],arguments[2]);
    }
    if(routine_name.compare("integrate.rt") == 0)
    {
        return routines::integrate_rt(function_name,arguments[0],arguments[1]);
    }
    if(routine_name.compare("integrate.mc") == 0)
    {
        return routines::integrate_mc(function_name,arguments[0],arguments[1]);
    }
    if(routine_name.compare("integrate2d.rect") == 0)
    {
        return routines::integrate2d_rect(function_name,arguments[0],arguments[1],arguments[2],arguments[3]);
    }
    if(routine_name.compare("integrate2d.type1") == 0)
    {
        return routines::integrate2d_type1(function_name,arguments[0],arguments[1],aux_arguments);
    }
    if(routine_name.compare("integrate2d.type2") == 0)
    {
        return routines::integrate2d_type2(function_name,arguments[0],arguments[1],aux_arguments);
    }
    if(routine_name.compare("integrate2d.line") == 0)
    {
    return routines::integrate2d_line(function_name,arguments[0],arguments[1],aux_arguments);
    }
    if(routine_name.compare("integrate3d.line") == 0)
    {
    return routines::integrate3d_line(function_name,arguments[0],arguments[1],aux_arguments);
    }
    if(routine_name.compare("integrate3d.cub") == 0)
    {
        return routines::integrate3d_cub(function_name,arguments[0],arguments[1],arguments[2],arguments[3],arguments[4],arguments[5]);
    }
    if(routine_name.compare("integrate3d.surf") == 0)
    {
        return routines::integrate3d_surf(function_name,arguments[0],arguments[1],arguments[2],arguments[3],aux_arguments);
    }
    if(routine_name.compare("solve.n") == 0)
    {
        return routines::newton(function_name,arguments[0]);
    }
    if(routine_name.compare("solve.b") == 0)
    {
        return routines::bisection(function_name,arguments[0],arguments[1]);
    }
    //returns 0 as default
    return Number(0.0);
}

//IMPLEMENTATION OF THE CLASS NDARRAY
ndArray::ndArray()
{
    dim = 0;
    dim_size.reserve(DEFAULT_DIM);
}
//function to change a Complex_array in ndArray
void ndArray::get_ndarray(Complex_array& c_array)
{
    ndArray temp;
    temp.dim_size.reserve(2);
    temp.dim = 2;
    temp.dim_size[0] = c_array.size();
    temp.dim_size[1] = 2;
    std::vector<int> index(2);
    for(int i = 0;i < dim_size[0];i++)
    {
        index[0] = i;
        index[1] = 0;
        temp.store_value(index,c_array[i].real());
        index[1] = 1;
        temp.store_value(index,c_array[i].img());
    }
    //std::reverse(temp.dim_size.begin(),temp.dim_size.end());
    *this = temp;
    return;
}

void ndArray::store_value(std::vector<int> inp_index,Number value)
{
    array[inp_index] = value;
    return;
}
Number ndArray::return_value(std::vector<int> index)
{
    return array[index];
}
void ndArray::show_slice(std::queue<int> slice)
{
    std::vector<int> start_index  (dim,0);
    std::vector<int> end_index  (dim,0);
    if((int)slice.size() == dim)
    {
        int current_dim = -1;
        while(!slice.empty())
        {
            current_dim++;
            start_index[current_dim] = slice.front();
            slice.pop();
        }
        mpfr_printf("%.15Rf \n",return_value(start_index).value);
        return;
    }
    else
    {
        int current_dim = -1;
        while(!slice.empty())
        {
            current_dim++;
            start_index[current_dim] = slice.front();
            slice.pop();
            end_index[current_dim] = slice.front();
            slice.pop();
        }
        //is the slice is reversed
        if(!lexicographical_compare(start_index.begin(),start_index.end(),end_index.begin(),end_index.end()))
        {
            for(std::map<std::vector<int>,Number>::iterator it_array = array.find(start_index);
            it_array != array.find(end_index);
            it_array--)
            {
                mpfr_printf("%.15Rf \n",return_value(it_array -> first).value);

            }
            return;
        }

        for(std::map<std::vector<int>,Number>::iterator it_array = array.find(start_index);
            it_array != array.find(end_index);
            it_array++)
        {
            mpfr_printf("%.15Rf \n",return_value(it_array -> first).value);

        }
        return;
    }

}
void ndArray::show()
{
    //std::vector<int> dim_size_temp = dim_size;
    //std::reverse(dim_size.begin(),dim_size.end());

    int current_dim = -1;
    std::map<std::vector<int>,Number>::iterator it_array;

    for(it_array = array.begin();it_array != array.end();it_array++)
    {
        std::vector<int> index = it_array -> first;

        if(current_dim == -1)
        {
            std::cout<<" [ ";
            current_dim++;
        }
        while(current_dim < dim && current_dim != dim - 1)
        {

            std::cout<<" [ ";
            if((current_dim + 1) < dim)
            {
                current_dim++;
            }
            else
            {
                break;
            }
        }
        if(index[current_dim] == dim_size[current_dim] - 1)
        {
            mpfr_printf("%.15Rf",return_value(it_array -> first).value);
        }
        else
        {
            mpfr_printf("%.15Rf , ",return_value(it_array -> first).value);
        }

        while(index[current_dim] == dim_size[current_dim] - 1)
        {
            current_dim--;
            if(current_dim == -1 || index[current_dim] == dim_size[current_dim] - 1)
            {
                std::cout<<" ] ";
            }
            else
            {
                std::cout<<" ] ,";
            }
            if(current_dim == -1)
            {
                break;
            }
        }

    }
    std::cout<<std::endl;
    //dim_size = dim_size_temp;
    return;
}
void ndArray::array_def_parse(std::string expr,std::string::iterator it_expr)
{
    Token token;
    //required for unary minus
    Token previous_token;
    std::vector<int> index(dim,0);
    std::stack<Number> number_stack;
    int current_dim = -1;
    while(true)
    {
        it_expr = token.get_token(expr,it_expr);
        if(token.token_type == Token::SEMICOLON)
        {
            previous_token = token;
            return;
        }

        if(token.token_type == Token::SQ_LPAREN)
        {
            current_dim++;
            index[current_dim] = 0;
            previous_token = token;
            continue;
        }
        if(token.token_type == Token::OPERATOR && token.operator_id == Token::UNARY_MINUS)
        {
            previous_token = token;
            continue;
        }
        if(token.token_type == Token::NUMBER)
        {
            Number result;
            mpfr_set_str(result.value,token.token.c_str(),BASE,RND_MODE);
            if(previous_token.token_type == Token::OPERATOR)
            {
                result *= Number(-1.0);
            }
            store_value(index,result);
            index[current_dim]++;
            previous_token = token;
            continue;
        }
        if(token.token_type == Token::COMMA)
        {
            previous_token = token;
            continue;
        }
        if(token.token_type == Token::SQ_RPAREN)
        {
            current_dim--;
            if(current_dim == -1)
            {
                previous_token = token;
                return;
            }
            index[current_dim]++;
            previous_token = token;
            continue;
        }
        else
        {
            throw IMPROPER_EXPRESSION;
            return;
        }
    }
    return;
}

void ndArray::define_linspace(Number start,Number end,int num_points)
{
    std::vector<int> index(dim,0);
    Number _num_points;
    mpfr_set_ui(_num_points.value,num_points,RND_MODE);
    for(;index[0] < dim_size[0];index[0]++)
    {
        Number _index;
        mpfr_set_ui(_index.value,index[0],RND_MODE);
        Number value = start + ((end - start) / _num_points)*_index;
        store_value(index,value);
    }
    return;
}

void ndArray::read_from_file(const std::string in_filename)
{
    std::ifstream input_file;
    input_file.open(in_filename.c_str());
    std::string expr;
    if(!input_file.is_open())
    {
        throw FILE_OPEN_FAILED;
    }
    //copy the input file to a string expr
    input_file.seekg(0, std::ios::end);
    expr.resize(input_file.tellg());
    input_file.seekg(0, std::ios::beg);
    input_file.read(&expr[0], expr.size());
    input_file.close();

    //remove all whitespaces
    expr.erase(std::remove(expr.begin(), expr.end(), '\t'), expr.end());
    expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());
    expr.erase(std::remove(expr.begin(), expr.end(), '\n'), expr.end());

    std::string::iterator it_expr = expr.begin();
    array_def_parse(expr,it_expr);
    return;
}

void ndArray::write_to_file(const std::string out_filename)
{
    FILE* out_file;
    out_file = fopen(out_filename.c_str(),"w");
    if(out_file == NULL)
    {
        throw FILE_OPEN_FAILED;
    }

    //std::vector<int>dim_size_temp = dim_size;
    //std::reverse(dim_size.begin(),dim_size.end());

    int current_dim = -1;
    std::map<std::vector<int>,Number>::iterator it_array;

    for(it_array = array.begin();it_array != array.end();it_array++)
    {
        std::vector<int> index = it_array -> first;

        if(current_dim == -1)
        {
            fprintf(out_file," [ ");
            current_dim++;
        }
        while(current_dim < dim && current_dim != dim - 1)
        {

            fprintf(out_file," [ ");
            if((current_dim + 1) < dim)
            {
                current_dim++;
            }
            else
            {
                break;
            }
        }
        if(index[current_dim] == dim_size[current_dim] - 1)
        {
            mpfr_fprintf(out_file,"%Rf",return_value(it_array -> first).value);
        }
        else
        {
            mpfr_fprintf(out_file,"%Rf , ",return_value(it_array -> first).value);
        }

        while(index[current_dim] == dim_size[current_dim] - 1)
        {
            current_dim--;
            if(current_dim == -1 || index[current_dim] == dim_size[current_dim] - 1)
            {
                fprintf(out_file," ] ");
            }
            else
            {
                fprintf(out_file," ] ,");
            }
            if(current_dim == -1)
            {
                break;
            }

        }

    }
    fclose(out_file);
    //dim_size = dim_size_temp;
    return;

}

void ndArray::write_to_file_csv(const std::string out_filename)
{
    FILE* out_file;
    out_file = fopen(out_filename.c_str(),"w");
    if(out_file == NULL)
    {
        throw FILE_OPEN_FAILED;
    }

    //std::vector<int>dim_size_temp = dim_size;
    //std::reverse(dim_size.begin(),dim_size.end());

    int current_dim = -1;
    std::map<std::vector<int>,Number>::iterator it_array;

    for(it_array = array.begin();it_array != array.end();it_array++)
    {
        std::vector<int> index = it_array -> first;

        if(current_dim == -1)
        {
            //fprintf(out_file," [ ");
            current_dim++;
        }
        while(current_dim < dim && current_dim != dim - 1)
        {

            //fprintf(out_file," [ ");
            if((current_dim + 1) < dim)
            {
                current_dim++;
            }
            else
            {
                break;
            }
        }
        if(index[current_dim] == dim_size[current_dim] - 1)
        {
            mpfr_fprintf(out_file,"%Rf",return_value(it_array -> first).value);
        }
        else
        {
            mpfr_fprintf(out_file,"%Rf , ",return_value(it_array -> first).value);
        }

        while(index[current_dim] == dim_size[current_dim] - 1)
        {
            current_dim--;
            if(current_dim == -1 || index[current_dim] == dim_size[current_dim] - 1)
            {
                fprintf(out_file,"\n");
            }
            else
            {
                fprintf(out_file,"\n");
            }
            if(current_dim == -1)
            {
                break;
            }

        }

    }
    fclose(out_file);
    //dim_size = dim_size_temp;
    return;

}

//evaluates the single variable function on the whole array
void ndArray::evaluate(std::string function_name,std::string output_array_name)
{

    //check whether the function has been defined
    if(map_functions.count(function_name) > 0)
    {
        //check if the function is a single variable function
        if(map_functions[function_name].num_arguments != 1)
        {
            std::cout<<"Evaluate can only be defined for single variable functions."<<std::endl;
            return;
        }
        else
        {
            for(std::map<std::vector<int>,Number>::iterator it_array = array.begin();
            it_array != array.end();
            it_array++)
            {
                std::vector<Number> arguments(1);
                arguments[0] = array[it_array -> first];
                map_ndarrays[output_array_name].store_value(it_array -> first,map_functions[function_name].evaluate(arguments));
            }

        }
    }
    else
    {
        std::cout<<function_name<<" : "<<FUNCTION_NOT_DEFINED<<std::endl;

    }
    return;

}

void ndArray::set2d(Number** A)
{
    if(dim != 2)
    {
        throw DIM_MISMATCH;
        return;
    }
    std::map<std::vector<int>,Number>::iterator it = array.begin();
    for(unsigned long i = 0;(int)i < dim_size[0];i++)
    {
        for(unsigned long j = 0;(int)j < dim_size[1];j++)
        {
            A[i][j] = it -> second;
            it++;
        }
    }
    return;
}

void ndArray::set1d(Number* A)
{
    if(dim != 1)
    {
        throw DIM_MISMATCH;
        return;
    }
    std::map<std::vector<int>,Number>::iterator it = array.begin();
    for(unsigned long i = 0;(int)i < dim_size[0];i++)
    {
        A[i] = it -> second;
        it++;
    }
    return;

}

void ndArray::get1d(Number* A,unsigned long size)
{
    dim = 1;
    dim_size[0] = size;
    std::vector<int> index(1);
    for(int i = 0; i < (int)size;i++)
    {
        index[0] = i;
        array[index] = A[i];
    }
    return;
}

//IMPLEMENTATION OF THE CLASS NUMBER
Number::Number()
{
    mpfr_init(value);
    mpfr_set_zero(value,0);
}
Number::Number(cppdouble _value)
{
    mpfr_init(value);
    mpfr_set(value,_value,MPFR_RNDN);

}
Number::Number(double _value)
{
    mpfr_init(value);
    mpfr_set_d(value,_value,MPFR_RNDN);
}
Number::Number(int _value)
{
    mpfr_init(value);
    mpfr_set_si(value,_value,MPFR_RNDN);
}
Number Number::operator+(const Number num2)
{
    Number result;
    mpfr_add(result.value,value,num2.value,MPFR_RNDN);
    return result;
}

Number Number::operator-(const Number num2)
{
    Number result;
    mpfr_sub(result.value,value,num2.value,MPFR_RNDN);
    return result;
}

Number Number::operator*(const Number num2)
{
    Number result;
    mpfr_mul(result.value,value,num2.value,MPFR_RNDN);
    return result;
}

Number Number::operator/(const Number num2)
{
    Number result;
    mpfr_div(result.value,value,num2.value,MPFR_RNDN);
    return result;
}

Number Number::operator%(const Number num2)
{
    mpz_t n1,n2,r;
    mpz_init(n1);
    mpz_init(n2);
    mpz_init(r);
    mpfr_get_z(n1,value,RND_MODE);
    mpfr_get_z(n2,num2.value,RND_MODE);
    mpz_tdiv_r(r,n1,n2);
    Number result;
    mpfr_set_z(result.value,r,RND_MODE);
    mpz_clears(n1,n2,r);
    return result;
}

Number Number::operator-()
{
    Number result;
    mpfr_neg(result.value,value,MPFR_RNDN);
    return result;
}


Number Number::operator^(const Number num2)
{
    Number result;
    mpfr_pow(result.value,value,num2.value,MPFR_RNDN);
    return result;
}

Number Number::operator+=(const Number num2)
{
    *this = *this + num2;
    return *this;
}
Number Number::operator-=(const Number num2)
{
    *this = *this - num2;
    return *this;
}
Number Number::operator*=(const Number num2)
{
    *this = *this * num2;
    return *this;
}
Number Number::operator/=(const Number num2)
{
    *this = *this / num2;
    return *this;
}
Number Number::operator++()
{
    *this = *this + Number(1.0);
    return *this;
}
Number Number::operator--()
{
    *this = *this - Number(1.0);
    return *this;
}
Number Number::operator=(const double num2)
{
    return Number(num2);
}
bool Number::operator>(const Number num2)
{
    int result = mpfr_cmp(value,num2.value);
    if(result > 0)
    {
        return true;
    }
    return false;
}

bool Number::operator<(const Number num2)
{
    int result = mpfr_cmp(value,num2.value);
    if(result < 0)
    {
        return true;
    }
    return false;
}
bool Number::operator>=(const Number num2)
{
    int result = mpfr_cmp(value,num2.value);
    if(result > 0 || result == 0)
    {
        return true;
    }
    return false;
}
bool Number::operator<=(const Number num2)
{
    int result = mpfr_cmp(value,num2.value);
    if(result < 0 || result == 0)
    {
        return true;
    }
    return false;
}
bool Number::operator==(const Number num2)
{
    int result = mpfr_cmp(value,num2.value);
    if(result == 0)
    {
        return true;
    }
    return false;
}
bool Number::operator!=(const Number num2)
{
    return !(*this == num2);
}

bool Number::operator==(const double _num2)
{
    Number num2(_num2);
    int result = mpfr_cmp(value,num2.value);
    if(result == 0)
    {
        return true;
    }
    return false;
}







