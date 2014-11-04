#ifndef MISC_H
#define MISC_H

#include "parser.hpp"
#include <string>

bool is_semicolon(const char c);
bool is_colon(const char c);
bool is_operator(const char c);
bool is_lparen(const char c);
bool is_rparen(const char c);
bool is_sq_lparen(const char c);
bool is_sq_rparen(const char c);
bool is_comma(const char c);
bool is_define(std::string token);
bool is_equal_sign(const char c);
bool is_routine(std::string token);
bool is_showrpn(std::string token);
bool is_linspace(std::string token);
bool is_zeros(std::string token);
bool is_ones(std::string token);
bool is_write(std::string token);
bool is_read(std::string token);
bool is_evaluate(std::string token);

#endif
