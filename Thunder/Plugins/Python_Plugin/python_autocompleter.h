#ifndef PYTHON_AUTOCOMPLETER_H
#define PYTHON_AUTOCOMPLETER_H

#include <memory>

#include "python_parser.h"
// #include "../auto_complete_token.h"
#include "../plugin_tokens.h"
#include "../plugins_helper_function.h"

class PythonAutoCompleter{
    public:
        PythonBlock* current_block; //the block where the cursor is in
        Python_Token* current_token;

        PythonAutoCompleter(PythonBlock* block,Python_Token* current_token_){
            current_block = block;
            current_token = current_token_;
        }

        std::vector<std::string> function_parameters(PythonBlock* block,std::string name){
            //parameters will not be present in current block but in its parent block where the function was registered
            std::vector<std::string> params;
            PythonBlock* b = block->parent_block;
            if (b){
                for (PythonDeclaration* tok:b->declaration_statements){
                    if (tok->name == name && tok->type == PYTHON_DECLARATION_TYPE::PYTHON_FUNCTION) return tok->parameters;
                }
            }
            return params;
        }

        std::vector<std::unique_ptr<AutoCompleteToken>> auto_complete_tokens(){
            std::vector<std::unique_ptr<AutoCompleteToken>> auto_tok;
            if (current_token && current_token->token_type == PYTHON_TOKEN_TYPES::IDENTIFIER){
                PythonBlock* block = current_block;
                PluginHelper matcher;

                while (block){
                    for (PythonDeclaration* dec:block->declaration_statements){
                        // if (matcher.word_matcher(dec->name,current_token->raw_data)){
                        if (matcher.word_matcher(current_token->raw_data,dec->name)){
                            //if name match current word with more than 0%
                            if (dec->type == PYTHON_DECLARATION_TYPE::PYTHON_FUNCTION){
                                //it is a function
                                auto_tok.push_back(std::make_unique<AutoCompleteToken>(AutoCompleteToken{dec->name, dec->name + "()"}));

                            }
                            else auto_tok.push_back(std::make_unique<AutoCompleteToken>(AutoCompleteToken{dec->name, dec->name}));
                        }
                        for (std::string param : dec->parameters){
                            if (matcher.word_matcher(param,current_token->raw_data)) auto_tok.push_back(std::make_unique<AutoCompleteToken>(AutoCompleteToken{param, param}));
                        }

                    }
                    block = block->parent_block;
                }
            }
            return auto_tok;
        }

};


#endif