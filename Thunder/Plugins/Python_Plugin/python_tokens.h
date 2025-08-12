//TODO : support ''' ''' string also
//TODO : i am using float and int but in python numbers can be more larger


#ifndef PYTHON_TOKENS_H
#define PYTHON_TOKENS_H

#include <string>
#include <variant>

enum class PYTHON_TOKEN_TYPES{
    KEYWORD, // keywords like for ,print
    IDENTIFIER, // identifier like variable name function name
    STRING, // words starting with "  or ' and ending with " or '
    NUMBER, //integer,float etc
    // OPERATOR, // like +,-,^
    ASSIGNMENT_OPERATOR,
    COMPARISON_OPERATOR,
    BITWISE_OPERATOR,
    ARITHMETIC_OPERATOR,
    L_PARENTHESIS, // (
    R_PARENTHESIS, // )
    L_BRACKET, // {
    R_BRACKET, // }
    LSQ_BRACKET, // [
    RSQ_BRACKET, // ]
    NEWLINE, // is important for python 
    INDENT, // as it is required in python
    COMMA, // ,
    COLON, // :
    // NONE, // None
    PYTHON_ERROR
};

struct Python_Token{
    std::string raw_data;
    std::variant<std::string,float> value;
    PYTHON_TOKEN_TYPES token_type;
    int startLine;
    int endLine;
    int start;
    int end;

    Python_Token(std::string raw_data_,std::string data,PYTHON_TOKEN_TYPES type,int sy,int ey,int sx,int ex){
        raw_data = raw_data_;
        // s_data = data;
        value = data;
        token_type = type;
        startLine = sy;
        endLine = ey;
        start = sx;
        end = ex;
    }

    Python_Token(std::string raw_data_,float data,PYTHON_TOKEN_TYPES type,int sy,int ey,int sx,int ex){
        raw_data = raw_data_;
        value = data;
        token_type = type;
        startLine = sy;
        endLine = ey;
        start = sx;
        end = ex;
    }

    operator std::string()const{
        return std::get<std::string>(value);
    }

    operator float()const{
        return std::get<float>(value);
    }

};

#endif