#include<string>
#include<iostream>

//checks whether the char is an operator
bool is_operator(const char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||c == '^' ||c == '!');
}

//checks whether the char is a left parentheses
bool is_lparen(const char c)
{
    return (c == '(');
}

//checks whether the char is a right parentheses
bool is_rparen(const char c)
{
    return (c == ')');
}

//checks if the char is function argument separator ','
bool is_comma(const char c)
{
    return (c == ',');
}

//checks whether the expr has a define keyword followed by a space
//from the given position
//DESIGN A BETTER SOLUTION THAN THIS NONSENSE
bool is_define(const char c1,const char c2,const char c3,const char c4,const char c5,const char c6,const char c7)
{
    if(c1 == 'd' && c2 == 'e' && c3 == 'f' && c4 == 'i' && c5 == 'n' && c6 == 'e' && c7 == ' ')
    {
        return true;
    }
    else
    {
        return false;
    }

}

//checks whether the char is an equal '=' sign
bool is_equal_sign(const char c)
{
    return (c == '=');
}
