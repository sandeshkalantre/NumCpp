/*
This file contains the macros for the error messages
To make changes in the error messages the string must be changed here
*/
#ifndef ERRORS_H
#define ERRORS_H

#define UNKNOWN_TOKEN "Parsing Error : Unknown token in the given input."
#define MISSING_LEFT_PARENTHESES "Parsing Error : Left parentheses missing in the given input."
#define MISSING_RIGHT_PARENTHESES "Parsing Error : Right parentheses missing in the given input."
#define MISSING_PARENTHESES "Parsing Error : Parentheses missing in the given input"
#define MISSING_EQUAL_SIGN "Parsing Error : Equal sign missing in the define expression."
#define FUNCTION_NOT_DEFINED "Definition Error :The function has not been defined."
#define FILE_OPEN_FAILED "File Error : The program failed to open the file.\nDo you have read/write permission in the current directory?"
#define ARRAY_NOT_DEFINED "Definition Error : Array has not been defined"
#define VARIABLE_NOT_DEFINED "Definition Error : Variable has not been defined."
#define IMPROPER_EXPRESSION "Expression Error : The expression could not be parsed."
#define INVALID_EXPRESSION "Expression Error : Too many/too few values were present on the number stack while evaluation"
#define NOT_A_ROUTINE "Definition Error : Routine has not been defined in the program"
#define DIVISION_BY_ZERO "Domain Error : Division by zero."
#define NEGATIVE_FACTORIAL "Domain Error : Factorial of a neagtive number is not defined.\nUse gamma(number) for the gamma function."
#define WRONG_PRECISION "Precision Error : Precison must be between 10 and 1000000."
#define WRONG_PRINT_PRECISION "Precision Error : Print precision must be between 6 and 100000."
#define DIM_MISMATCH "Dimension Mismatch : The dimensions of the input array were not proper."
#define DISCONT_ERROR "Discontinuity : Given function must be continuous, but it appears this function is discontinuous."
#define DOMAIN_ERROR "Domain erorr : The function is undefined in the given domain."
#define INPUT_ERROR "Input Error : Given points must have function values of opposite sign for this method, but sadly they do not."
#define MALLOC_ERROR "Memory Error : Failed to allocate memory on the heap."
#define ARGUMENT_ERROR "Argument Error : The given argument is not valid."

#endif
