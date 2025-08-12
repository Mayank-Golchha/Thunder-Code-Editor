// TODO : doesnot support dict and class 
// TODO : i think it only create new block dont go back
// TODO : if a block overs and a new block starts the old block is not refenced by anyone

#ifndef PYTHON_PARSER_H
#define PYTHON_PARSER_H

#include <vector>
#include <iostream>

#include "python_ast.h"

class PythonParser{
    public:
        std::vector<Python_Token*> tokens;
        int len = 0,index = -1;
        Python_Token* current_token; // used by parser

        PythonBlock* global_block = NULL; //global 
        PythonBlock* current_block = NULL; //current
        pystack indentation_stack = NULL;
        Python_Token* selected_token = NULL; //for use of autocompleter
        PythonBlock* selected_block = NULL;  //the block which contain selected_token


        // PythonParser(std::vector<Python_Token*>& tokens_,Python_Token* selected_token_){
        PythonParser(std::vector<Python_Token*>& tokens_,Python_Token* selected_token_,int c_x,int c_y){
            tokens = tokens_;
            len = tokens.size();
            selected_token = selected_token_;
            // selected_token = getCurrentToken(c_x,c_y);

            global_block = new PythonBlock;
            current_block = global_block;
            pushPythonStack(&indentation_stack,0);
            advance();
        }

        Python_Token* getCurrentToken(int cx,int cy){
            int low = 0,high = tokens.size()-1,mid = 0;
            while (low <= high){
                mid = low + (high-low)/2;
                if (tokens[mid]->startLine == cy || tokens[mid]->endLine == cy){
                    int i = mid-1,j = mid+1;
                    while (i >= 0 && tokens[i]->startLine == cy){
                        if (cx > tokens[i]->start && cx <= tokens[i]->end && cy >= tokens[i]->startLine && cy <= tokens[i]->endLine) return tokens[i];
                        i--;
                    }

                    while (j < tokens.size() && tokens[j]->startLine == cy){
                        if (cx > tokens[j]->start && cx <= tokens[j]->end && cy >= tokens[j]->startLine && cy <= tokens[j]->endLine) return tokens[j];
                        j++;
                    }
                    return NULL;
                }
                else if (cy > tokens[mid]->startLine) low = mid+1;
                else high = mid-1;
            }
            
            return NULL;
        }

        void advance(){
            if (index + 1 >= len){
                current_token = NULL;
                return ;
            }
            index++;
            current_token = tokens[index];
            if (selected_token == current_token){
                // selected_block = current_block->parent_block;
                selected_block = current_block;
            }
        }

        std::string getRawData(Python_Token* token){
            if (token){
                return token->raw_data;
            }
            return "";
        }

        bool check_type(Python_Token* tok,PYTHON_TOKEN_TYPES type){
            if (!tok) return 0;

            return tok->token_type == type;
        }

        bool check_value(Python_Token* token,std::string data){
            //will check for raw data
            if (!token) return 0;
            return token->raw_data == data;
        }

        void register_variable(Python_Token* token){
            //will register token in current block
            if (!token) return;

            // PythonDeclaration* var = new PythonDeclaration{token->raw_data,0,PYTHON_DECLARATION_TYPE::PYTHON_VARIABLE};
            PythonDeclaration* var = new PythonDeclaration(token->raw_data,PYTHON_DECLARATION_TYPE::PYTHON_VARIABLE);
            current_block->declaration_statements.insert(var);
        }

        void register_function(std::string name,std::vector<std::string>* params){
            //will create a new block in current block and register the function
            //and will set new block as current block
            PythonBlock* block = new PythonBlock;
            block->parent_block = current_block;
            PythonDeclaration* func = new PythonDeclaration(name,PYTHON_DECLARATION_TYPE::PYTHON_FUNCTION,block,params);
            current_block->declaration_statements.insert(func);

            current_block = block;
        }

        void register_new_block(){
            // will create a new block and set as current block
            // will also increase indentation block
            PythonBlock* block = new PythonBlock;
            block->parent_block = current_block;
            current_block = block;
        }

        // void skip_space_tokens(){
        int skip_space_tokens(){
            //will skip space tokens
            int c = 0;
            while (check_type(current_token,PYTHON_TOKEN_TYPES::INDENT)) {
                advance();
                c++;
            }
            return c;
        }

        int varInBlock(PythonBlock* block,std::string data,PYTHON_DECLARATION_TYPE *type){
            //will check if identifier in given block or in given blocks parameters
            if (data == "") return 0;
            for (PythonDeclaration* tok:block->declaration_statements){
                // if (tok->name == data || varInParameters(tok,data)) return 1;
                if (tok->name == data){
                    *type = tok->type;
                    return 1;
                }
                else if (varInParameters(tok,data)){
                    *type = PYTHON_DECLARATION_TYPE::PYTHON_PARAMTER;
                    return 1;
                }
            }
            return 0;
        }

        int varInParameters(PythonDeclaration* dec,std::string name){
            //will check if variable name in block params;
            for (int i = 0;i<dec->parameters.size();i++){
                if (name == dec->parameters[i]) return 1;
            }
            return 0;
        }

        int parameterLookup(PythonBlock* block,std::string data){
            // will check if data in parameter in given block or in its parent block
            PythonBlock* temp = block;
            while (temp){
                for (PythonDeclaration* tok:block->declaration_statements){
                    if (varInParameters(tok,data)) return 1;
                }
                temp = temp->parent_block;
            }
            return 0;
        }

        int varLookup(PythonBlock* block,std::string data,PYTHON_DECLARATION_TYPE* type){
            // will check if var in given block or global block
            // return varInBlock(block,token) || varInBlock(global_block,token);
            if (varInBlock(block,data,type)) return 1;
            PythonBlock* temp = block->parent_block;

            while (temp){
                if (varInBlock(temp,data,type)) return 1;
                temp = temp->parent_block;
            }
            return 0;
        }

        int term(){
            // LPARENTHESIS + <expression> + RPARENTHESIS | <list_expression> | IDENTIFIER_R | NUMBER | STRING 
            if (check_type(current_token,PYTHON_TOKEN_TYPES::STRING) || check_type(current_token,PYTHON_TOKEN_TYPES::NUMBER)){
                advance();
                skip_space_tokens();
                return 1;
            }

            if (builtin_data_structure()) return 1; //will eat list or set or tuple or dict

            if (check_type(current_token,PYTHON_TOKEN_TYPES::IDENTIFIER)){
                PYTHON_DECLARATION_TYPE var_type;
                if (varLookup(current_block,getRawData(current_token),&var_type)){
                    if (var_type == PYTHON_DECLARATION_TYPE::PYTHON_VARIABLE || var_type == PYTHON_DECLARATION_TYPE::PYTHON_PARAMTER){
                        // it is a variable
                        advance();
                        skip_space_tokens();
                        return 1;
                    }
                    else if (var_type == PYTHON_DECLARATION_TYPE::PYTHON_FUNCTION){
                        // it is a function so must have ()
                        advance();
                        skip_space_tokens();
                        if (check_type(current_token,PYTHON_TOKEN_TYPES::L_PARENTHESIS)){
                            advance();
                            skip_space_tokens();
                            std::vector<std::string> params;

                            function_parameters(&params);

                            if (check_type(current_token,PYTHON_TOKEN_TYPES::R_PARENTHESIS)){
                                advance();
                                skip_space_tokens();
                                return 1;
                            }

                        }
                    }
                }
            }
            return 0;
        }

        void eat_op_e(){
            // OPERATOR + <term> + <op_e> | NONE
            //maybe a bug because not checking if term
            if (check_type(current_token,PYTHON_TOKEN_TYPES::ARITHMETIC_OPERATOR)){
                advance();
                skip_space_tokens();
                term();
                eat_op_e();
            }
        }

        int comma_exp(){
            // COMMA + <expression> + <comma_exp> | NONE
            if (check_type(current_token,PYTHON_TOKEN_TYPES::COMMA)){
                advance();
                skip_space_tokens();
                if (expression()){
                    return comma_exp();
                }
                return 0;
            }
            else return 1;
        }

        int comma_id(std::vector<std::string>* parameters){
            // COMMA + IDENTIFIER + <comma_id> | NONE
            if (check_type(current_token,PYTHON_TOKEN_TYPES::COMMA)){
                advance();
                skip_space_tokens();
                if (check_type(current_token,PYTHON_TOKEN_TYPES::IDENTIFIER)){
                    parameters->push_back(current_token->raw_data);
                    advance();
                    skip_space_tokens();
                    return comma_id(parameters);
                }
                return 0;
            }
            else return 1;
        
            return 0;
        }

        int function_parameters(std::vector<std::string>* parameters){
            // IDENTIFIER + comma_id | NONE
            if (check_type(current_token,PYTHON_TOKEN_TYPES::IDENTIFIER)){
                parameters->push_back(current_token->raw_data);
                advance();
                skip_space_tokens();
                if (comma_id(parameters)){
                    return 1;
                }
            }
            else return 1;

            return 0;
        }

        int function(){
            // def + IDENTIFIER + LPARENTHESIS + parameters + RPARENTHESIS + COLON
            // current token is def
            if (check_type(current_token,PYTHON_TOKEN_TYPES::IDENTIFIER)){
                if (!current_token) return 0;
                std::string name = current_token->raw_data;
                advance();
                skip_space_tokens();
                if (check_type(current_token,PYTHON_TOKEN_TYPES::L_PARENTHESIS)){
                    advance();
                    skip_space_tokens();
                    std::vector<std::string>* parameters = new std::vector<std::string>;
                    if (function_parameters(parameters)){
                        if (check_type(current_token,PYTHON_TOKEN_TYPES::R_PARENTHESIS)){
                            advance();
                            skip_space_tokens();
                            if (check_type(current_token,PYTHON_TOKEN_TYPES::COLON)){
                                if (indentation_stack){
                                    pushPythonStack(&indentation_stack,indentation_stack->data+1);
                                }
                                else pushPythonStack(&indentation_stack,1);
                                advance();
                                skip_space_tokens();
                                register_function(name,parameters);
                                return 1;
                            }
                        }
                    }
                }
            }

            return 0;
        }

        int is_data_structure(PYTHON_TOKEN_TYPES end_type){
            //will be used to identify list tuple set
            if (expression()){
                if (comma_exp()){
                    if (check_type(current_token,end_type)){
                        advance();
                        skip_space_tokens();
                        return 1;
                    }
                }
            }
            return 0;
        }

        int list_expression(){
            // LSQBRACKET + <expression> + <comma_exp> + RSQBRACKET 
            // current token is [
            return is_data_structure(PYTHON_TOKEN_TYPES::RSQ_BRACKET);
        }

        int tuple_expression(){
            // LPARENTHESIS + <expression> + <comma_exp> + RPARENTHESIS 
            // current token is (
            return is_data_structure(PYTHON_TOKEN_TYPES::R_PARENTHESIS);
        }

        int set_expression(){
            // LBRACKET + <expression> + <comma_exp> + RBRACKET 
            //current token is {
            return is_data_structure(PYTHON_TOKEN_TYPES::R_BRACKET);
        }

        int builtin_data_structure(){
            //will check if expression is a one of set,tuple,list or dict
            if (!current_token) return 0;
            switch (current_token->token_type){
                case PYTHON_TOKEN_TYPES::L_PARENTHESIS:
                    advance();
                    skip_space_tokens();
                    return tuple_expression();

                case PYTHON_TOKEN_TYPES::L_BRACKET:
                    advance();
                    skip_space_tokens();
                    return set_expression();

                case PYTHON_TOKEN_TYPES::LSQ_BRACKET:
                    advance();
                    skip_space_tokens();
                    return list_expression();
            }
            return 0;
        }

        int expression(){
            // <term> + <op_e>  
            if (term()){
                eat_op_e();
                return 1;
            }
            return 0;
        }

        void variable(){
            // IDENTIFIER + EQUAL + expression
            Python_Token* var = current_token;
            advance();
            skip_space_tokens();
            if (check_type(current_token,PYTHON_TOKEN_TYPES::ASSIGNMENT_OPERATOR) && check_value(current_token,"=")){
                advance();
                skip_space_tokens();
                if (expression()) register_variable(var);
            }
        }

        void parse(){
            while (current_token){
                if (check_type(current_token,PYTHON_TOKEN_TYPES::IDENTIFIER)){
                    variable();
                }

                else if (check_value(current_token,"def")){
                    //def is a keyword
                    advance();
                    skip_space_tokens();
                    function();
                }

                else if (check_type(current_token,PYTHON_TOKEN_TYPES::COLON)){
                    // if current token is : create a new block
                    // register_new_block();
                    advance();
                    skip_space_tokens();
                    if (check_type(current_token,PYTHON_TOKEN_TYPES::NEWLINE)){
                        advance();
                        register_new_block();
                        int c = skip_space_tokens();
                        // pushPythonStack(&indentation_stack,indentation_stack->data+1);
                        pushPythonStack(&indentation_stack,c);

                    }
                }

                else if (check_type(current_token,PYTHON_TOKEN_TYPES::NEWLINE)){
                    advance();
                    int count = skip_space_tokens();
                    
                    if (!indentation_stack) return ;
                    
                    int d = indentation_stack->data - count;
                    
                    if (count && count < indentation_stack->data){
                        popPythonStack(&indentation_stack);
                        while (indentation_stack && count != indentation_stack->data){
                            popPythonStack(&indentation_stack);
                        }
                    }
                }

                else advance();
            }
        }



};

#endif