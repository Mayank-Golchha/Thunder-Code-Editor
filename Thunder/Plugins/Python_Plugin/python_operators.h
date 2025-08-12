#ifndef PYTHON_OPERATORS_H
#define PYTHON_OPERATORS_H

#include <vector>
#include <string>

std::vector<std::string> python_comparison_operator = {"==", ">=", "<=", "!=", ">", "<"};
std::vector<std::string> python_assignment_operator = {"+=" , "-=", "*=", "/=", "%=", "//=", "**=", "&=" , "|=" , "^=", "<<=", ">>=", ":=","="};
std::vector<std::string> python_bitwise_operator = {"&", "|", "^", "~", "<<", ">>"};
std::vector<std::string> python_arithmetic_operator = {"+", "-", "*", "/", "%", "**","//"};

#endif