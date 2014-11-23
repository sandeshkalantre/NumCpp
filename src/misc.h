/*
Header file for the miscellaneous functions which are used to determine the token
type.Make changes here if one wants to change what a token represents.

Used only in get_token in class Token
*/
#ifndef MISC_H
#define MISC_H

#include "parser.hpp"
#include <string>

//the various is_* functions check if the argument is same as *
//return true if they match, false otherwise
bool is_semicolon(const char c);
bool is_colon(const char c);
bool is_operator(const char c);
bool is_lparen(const char c);
bool is_rparen(const char c);
bool is_sq_lparen(const char c);
bool is_sq_rparen(const char c);
bool is_comma(const char c);
bool is_define(const std::string token);
bool is_equal_sign(const char c);
bool is_routine(const std::string token);
bool is_showrpn(const std::string token);
bool is_linspace(const std::string token);
bool is_zeros(const std::string token);
bool is_ones(const std::string token);
bool is_write(const std::string token);
bool is_read(const std::string token);
bool is_evaluate(const std::string token);
bool is_help(const std::string token);
bool is_fft(const std::string token);
bool is_solve(const std::string token);
bool is_sci(const std::string token);

#endif
