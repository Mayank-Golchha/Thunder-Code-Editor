#ifndef C_PARSER_H
#define C_PARSER_H

#include <vector>
#include <iostream>

#include "c_ast.h"

class C_Parser{
    public:
        std::vector<C_Token*> tokens;
        int len = 0,index = -1;
        C_Token* current_token;

        CBlock* global_block = NULL; //global 
        CBlock* current_block = NULL; //current
        cstack indentation_stack = NULL;
        C_Token* selected_token = NULL; //for use of autocompleter
        CBlock* selected_block = NULL;  //the block which contain selected_token

        // PythonParser(std::vector<Python_Token*>& tokens_,Python_Token* selected_token_){
        C_Parser(std::vector<C_Token*>& tokens_,C_Token* selected_token_,int c_x,int c_y){
            tokens = tokens_;
            len = tokens.size();
            selected_token = selected_token_;
            // selected_token = getCurrentToken(c_x,c_y);

            global_block = new CBlock;
            current_block = global_block;
            pushCStack(&indentation_stack,0);
            advance();
        }

        void advance(){
            if (index + 1 >= len){
                current_token = NULL;
                return ;
            }
            index++;
            current_token = tokens[index];
            if (selected_token == current_token){
                selected_block = current_block;
            }
        }

        std::string getRawData(C_Token* token){
            if (token){
                return token->raw_data;
            }
            return "";
        }

        bool check_type(C_Token* tok,C_TOKEN_TYPES type){
            if (!tok) return 0;

            return tok->token_type == type;
        }

        bool check_value(C_Token* token,std::string data){
            //will check for raw data
            if (!token) return 0;
            return token->raw_data == data;
        }

        void register_variable(std::string name){
            //will register token in current block
            CDeclaration* var = new CDeclaration(name,C_DECLARATION_TYPE::C_VARIABLE);
            current_block->declaration_statements.insert(var);
        }

        void register_new_block(){
            // will create a new block and set as current block
            CBlock* block = new CBlock;
            block->parent_block = current_block;
            current_block = block;
        }

        int term(){
            // LPARENTHESIS + <expression> + RPARENTHESIS | IDENTIFIER | NUMBER | STRING | CHAR
            if (check_type(current_token,C_TOKEN_TYPES::STRING) || check_type(current_token,C_TOKEN_TYPES::NUMBER) || check_type(current_token,C_TOKEN_TYPES::CHAR)){
                advance();
                return 1;
            }
            if (check_type(current_token,C_TOKEN_TYPES::LPARENTHESIS)){
                advance();
                if (expression()){
                    if (check_type(current_token,C_TOKEN_TYPES::RPARENTHESIS)){
                        advance();
                        return 1;
                    }
                }
                return 0;
            }
            return 0;
        }

        int eat_op_e(){
            // OPERATOR + <term> + <op_e> | NONE
            if (check_type(current_token,C_TOKEN_TYPES::ARITHMETIC_OPERATOR)){
                advance();
                if (term()) return eat_op_e();
                return 0;
            }
            return 1;
        }

        int expression(){
            // <term> + <op_e>
            if (term()){
                return eat_op_e();
            }
            return 0;
        }

        int equal_expression(){
            // EQUAL + <expression> | NONE
            if (check_value(current_token,"=")){
                advance();
                return expression();
            }
            return 1;
        }

        int variable_dec(){
            // <data_type> + <variable_assignment> + <multiple_var_dec>
            if (is_data_type(current_token)){
                advance();
                std::string name = getRawData(current_token);
                if (variable_assignment()){
                    register_variable(name);
                    return multiple_var_dec();
                }
            }
            return 0;
        }

        int variable_assignment(){
            // IDENTIFIER + <equal_value>
            if (check_type(current_token,C_TOKEN_TYPES::IDENTIFIER)){
                advance();
                if (equal_expression()){
                    return multiple_var_dec();
                }
            }
            return 0;
        }

        int multiple_var_dec(){
            // COMMA + <variable_assignment>
            if (check_type(current_token,C_TOKEN_TYPES::COMMA)){
                advance();
                std::string name = getRawData(current_token);
                if (variable_assignment()){
                    register_variable(name);
                    return 1;
                }
                return 0;
            }
            return 1;
        }

        int is_data_type(C_Token* token){
            // int | char | float | double | bool
            if (check_type(token,C_TOKEN_TYPES::KEYWORDS)){
                if (token->raw_data == "int" || token->raw_data == "char" || token->raw_data == "float" || token->raw_data == "double" || token->raw_data == "bool"){
                    return 1;                    
                }
            }
            return 0;
        }

        int variable(){
            // <variable_dec> + SEMICOLON
            if (variable_dec()){
                if (check_type(current_token,C_TOKEN_TYPES::SEMICOLON)){
                    advance();
                    return 1;
                }
            }
            return 0;
        }

        void parse(){
            while (current_token){
                if (check_type(current_token,C_TOKEN_TYPES::KEYWORDS)){
                    if (variable());
                }
                
                else if (check_type(current_token,C_TOKEN_TYPES::LBRACKET)){
                    register_new_block();
                    advance();
                }

                else if (check_type(current_token,C_TOKEN_TYPES::RBRACKET)){
                    if (current_block != global_block){
                        current_block = current_block->parent_block;
                    }
                    advance();
                }

                else advance();
            }
        }
};


#endif