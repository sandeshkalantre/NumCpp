#include "misc.h"

//checks whether the char is an operator
bool is_operator(const char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||c == '^' || c == '!' || c == 'e' || c == 'E');
}

//checks whether the char is a semicolon
bool is_semicolon(const char c)
{
    return(c == ';');
}

//checks whether the char is a colon
bool is_colon(const char c)
{
    return(c == ':');
}

//checks whether the char is a left parentheses
bool is_lparen(const char c)
{
    return (c == '(');
}

//checks whether the char is a square left parentheses
bool is_sq_lparen(const char c)
{
    return (c == '[');
}

//checks whether the char is a right parentheses
bool is_rparen(const char c)
{
    return (c == ')');
}

//checks whether the char is a square right parentheses
bool is_sq_rparen(const char c)
{
    return (c == ']');
}
//checks if the char is function argument separator ','
bool is_comma(const char c)
{
    return (c == ',');
}

//checks whether the char is an equal '=' sign
bool is_equal_sign(const char c)
{
    return (c == '=');
}

//checks whether the expr has a define keyword followed by a space
//from the given position
bool is_define(std::string token)
{
    if(token.compare("define") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

//checks whether the token is the name of a routine
bool is_routine(std::string token)
{
    return (map_routines.count(token) > 0);
}

//checks whether the token is showrpn keyword
bool is_showrpn(std::string token)
{
    if(token.compare("showrpn") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//checks whether the token is linspace keyword
bool is_linspace(std::string token)
{
    if(token.compare("linspace") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//checks whether the token is the keyword ones
bool is_ones(std::string token)
{
    if(token.compare("ones") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//checks whether the token is the keyword zeros
bool is_zeros(std::string token)
{
    if(token.compare("zeros") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}



