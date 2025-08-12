#ifndef LEXER_H
#define LEXER_H

#include <vector>

#include "tokens.h"

class Lexer{
    public:
        std::string code;
        char current_token;
        int index;
        int length;
        int line_no;
        int start_index; //will be reset when new line encounters

        Lexer(std::string code_){
            code = code_;
            index = -1;
            current_token = '\0';
            length = code.size();
            line_no = 0;
            start_index = 0;
            advance();
        }

        void advance(){
            if (index+1 < length){
                index++;
                current_token = code[index];
                start_index++;
                return ;
            }
            current_token = '\0';
        }

        JSON_PARSER::Token* string_token(){
            //if u send to this function
            //it is assumed that token is a string
            //starting character is ' or "

            std::string value;
            
            int i = index;

            JSON_PARSER::Token* token = new JSON_PARSER::Token;

            char s = '\'';
            if (current_token == '"') s = '"';

            // value += current_token;            
            advance();
        
            while (current_token && current_token != s){
                if (current_token == '\n'){
                    token->value = code.substr(i,index-i);
                    token->token_type = TOKEN_TYPES::ERROR_T;
                    advance();
                    return token;
                }
                advance();
                if (!current_token){
                    throw std::runtime_error("[ERROR] : INVALID JSON");
                    // token->value = code.substr(i,index-i);
                    // token->token_type = TOKEN_TYPES::ERROR_T;
                    // return token;
                }
            }
            if (current_token) value = code.substr(i+1,index-i-1);
            // else value = code.substr(i+1,index-i-1); //missing ' or "
            else throw std::runtime_error("[ERROR] : INVALID JSON");
            advance();
         
            token->value = value;
            
            token->token_type = TOKEN_TYPES::STRING_T;

            return token;

        }

        JSON_PARSER::Token* number(){
            //if u send to this function
            //it is assumed that token is a string

            std::string no;
            no += current_token;
            int start = index;
            int dot = 0;

            JSON_PARSER::Token* t = new JSON_PARSER::Token;

            int e = 0,s = 0; //e for expononet and s for sign
            advance();

            while (current_token){
                if (current_token >= '0' && current_token <= '9') advance();
                else if (current_token == '.' && dot <= 1){
                    dot += 1;
                    advance();
                }
                else if (current_token == 'e'){
                    e += 1;
                    advance();
                }
                else if ((current_token == '+' || (current_token == '-') && e == 1 && !s)){
                    s++;
                    advance();
                }
                else break;
            }
            if (dot < 2 && index < length && e < 2) {
                no += code.substr(start+1,index-start);
                t->token_type = TOKEN_TYPES::NUMBER_T;
            }
            else if (start < length) {
                throw std::runtime_error("[ERROR] : INVALID ERROR");
                // no = code.substr(start,length-start); //error
                // t->token_type = TOKEN_TYPES::ERROR_T;
            }
            else {
                // code = " "; //error
                throw std::runtime_error("[ERROR] : INVALID ERROR");

                // t->token_type = TOKEN_TYPES::ERROR_T;
            }

            t->value = no;
            return t;
        }

        std::vector<JSON_PARSER::Token*> tokenize(){
            std::vector<JSON_PARSER::Token*> tokens;
            //
            size_t pos = code.find('{');
            if (pos != std::string::npos) {
                code = code.substr(pos); // Return from the first `{` to the end
            }
            else return {};
            index--;
            start_index--;
            advance();
            // 
            while (current_token){
                if (current_token == ' ' || current_token == '\t') advance();
                else if (current_token == '\n'){
                    line_no++;
                    start_index = 0;
                    // index = 0;
                    advance();
                }
                else if (current_token == '{'){
                    JSON_PARSER::Token* t = new JSON_PARSER::Token{"{",TOKEN_TYPES::LBRACKET_T};
                    tokens.push_back(t);
                    advance();
                }
                else if (current_token == '}'){
                    JSON_PARSER::Token* t = new JSON_PARSER::Token{"}",TOKEN_TYPES::RBRACKET_T};
                    tokens.push_back(t);
                    advance();
                }
                else if (current_token == '['){
                    JSON_PARSER::Token* t = new JSON_PARSER::Token{"[",TOKEN_TYPES::LSQBRACKET_T};
                    tokens.push_back(t);
                    advance();
                }
                else if (current_token == ']'){
                    JSON_PARSER::Token* t = new JSON_PARSER::Token{"]",TOKEN_TYPES::RSQBRACKET_T};
                    tokens.push_back(t);
                    advance();
                }
                else if (current_token == ','){
                    JSON_PARSER::Token* t = new JSON_PARSER::Token{",",TOKEN_TYPES::COMMA_T};
                    tokens.push_back(t);
                    advance();
                }
                else if (current_token == ':'){
                    JSON_PARSER::Token* t = new JSON_PARSER::Token{":",TOKEN_TYPES::COLON_T};
                    tokens.push_back(t);
                    advance();
                }
                else if (current_token == '\'' || current_token == '"'){
                    tokens.push_back(string_token());
                }
                else if ((current_token >= '0' && current_token <= '9') || current_token == '-' || current_token == '+') tokens.push_back(number());
                else if (current_token == 't'){
                    JSON_PARSER::Token* t = new JSON_PARSER::Token;
                    if (index+3 < length && code.substr(index,4) == "true"){
                        t->token_type = TOKEN_TYPES::BOOLEAN_T;
                        t->value = "true";
                        index += 4;
                        current_token = code[index];
                    }
                    else{
                        t->value = current_token;
                        t->token_type = TOKEN_TYPES::ERROR_T;
                        advance();
                    }
                    tokens.push_back(t);
                }
                else if (current_token == 'f'){
                    JSON_PARSER::Token* t = new JSON_PARSER::Token;
                    if (index+4 < length && code.substr(index,5) == "false"){
                        t->token_type = TOKEN_TYPES::BOOLEAN_T;
                        t->value = "false";
                        index += 5;
                        current_token = code[index];
                    }
                    else{
                        t->value = current_token;
                        t->token_type = TOKEN_TYPES::ERROR_T;
                        advance();
                    }
                    tokens.push_back(t);

                }
                else if (current_token == 'n'){
                    JSON_PARSER::Token* t = new JSON_PARSER::Token;
                    if (index+3 < length && code.substr(index,4) == "null"){
                        t->token_type = TOKEN_TYPES::NONE_T;
                        t->value = "null";
                        index += 4;
                        current_token = code[index];
                    }
                    else{
                        t->value = current_token;
                        t->token_type = TOKEN_TYPES::ERROR_T;
                        advance();
                    }
                    tokens.push_back(t);

                }
                else {
                    JSON_PARSER::Token* t = new JSON_PARSER::Token{std::string(1,current_token),TOKEN_TYPES::ERROR_T};
                    tokens.push_back(t);
                    advance();
                }
            }

            return tokens;
        }
};

#endif