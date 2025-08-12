#ifndef C_TOKENS_H
#define C_TOKENS_H

#include <string>
#include <variant>

enum class C_TOKEN_TYPES{
    IDENTIFIER,
    NUMBER,
    STRING,
    CHAR,
    KEYWORDS,
    LPARENTHESIS,
    RPARENTHESIS,
    LBRACKET,
    RBRACKET,
    LSQBRACKET,
    RSQBRACKET,
    SEMICOLON,
    COLON,
    COMMA,
    ASSIGNMENT_OPERATOR,
    COMPARISON_OPERATOR,
    BITWISE_OPERATOR,
    ARITHMETIC_OPERATOR,
    PREPROCESSOR_DIRECTIVES,
    C_ERROR
};

struct C_Token{
    std::string raw_data;
    std::variant<std::string,float> value;
    C_TOKEN_TYPES token_type;
    int startLine;
    int endLine;
    int start;
    int end;

    C_Token(std::string raw_data_,std::string data,C_TOKEN_TYPES type,int sy,int ey,int sx,int ex){
        raw_data = raw_data_;
        value = data;
        token_type = type;
        startLine = sy;
        endLine = ey;
        start = sx;
        end = ex;
    }

    C_Token(std::string raw_data_,float data,C_TOKEN_TYPES type,int sy,int ey,int sx,int ex){
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