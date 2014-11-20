/*
This file contains the macros for the error messages
To make changes in the error messages the string must be changed here
*/
#ifndef ERRORS_H
#define ERRORS_H

#define UNKNOWN_TOKEN "Unknown token in the given input."
#define MISSING_LEFT_PARENTHESES "Left parentheses missing in the given input."
#define MISSING_RIGHT_PARENTHESES "Right parentheses missing in the given input."
#define MISSING_PARENTHESES "Parentheses missing in the given input"
#define MISSING_EQUAL_SIGN "Equal sign missing in the define expression."
#define FUNCTION_NOT_DEFINED "The function has not been defined."
#define FILE_OPEN_FAILED "The program failed to open the file.Do you have write permission in the current directory"
#define ARRAY_NOT_DEFINED "Array has not been defined"
#define VARIABLE_NOT_DEFINED "Variable has not been defined."
#define IMPROPER_EXPRESSION "The expression could not be parsed."
#define INVALID_EXPRESSION "Too many/too few values were present on the number stack while evaluation"
#define NOT_A_ROUTINE "Routine has not been defined in the program"
#define DIVISION_BY_ZERO "Division by zero."
#define NEGATIVE_FACTORIAL "Factorial of a neagtive number is not defined.\nUse gamma(number) for the gamma function."
#define WRONG_PRECISION "Precison must be between 10 and 1000000."
#define DIM_MISMATCH "The dimensions of the input array were not proper."

#endif
