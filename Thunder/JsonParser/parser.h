#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "dumper.h"

class Json{
    public:
        std::vector<JSON_PARSER::Token*> tokens;
        JSON_PARSER::Token* current_token;
        std::vector<void*> allocated_memory;
        int index;
        int len;
        
        Json(std::string code_){
            index = -1;
            tokens = Lexer(code_).tokenize();
            len = tokens.size();
            advance();
        }

        void advance(){
            if (index + 1 < len){
                index++;
                current_token = tokens[index];
                return;
            }
            current_token = NULL;
        }

        void clear(){
            for (int i = 0;i<tokens.size();i++){
                delete tokens[i];
            }
            // tokens.clear();
        }

        int JsonProperty(std::unordered_map<std::string,JsonNode>&map){
            if (current_token && current_token->token_type == TOKEN_TYPES::STRING_T){
                std::string key = current_token->value;
                advance();
                if (current_token && current_token->token_type == TOKEN_TYPES::COLON_T){
                    advance();
                    JsonNode* value = JsonValue();

                    if (value){
                        map[key] = *value;
                        return 1;
                    } 
                }
            }
            return 0;
        }

        void JsonChildrenProperty(std::unordered_map<std::string,JsonNode>&map){
            if (current_token && current_token->token_type == TOKEN_TYPES::COMMA_T){
                advance();
                if (JsonProperty(map)){
                    JsonChildrenProperty(map);
                }
                else{
                    //something must be there after comma
                    throw std::runtime_error("[ERROR] : INVALID JSON");
                }
            }
        }


        void JsonArrayChildren(std::vector<JsonNode>& array){
            if (current_token && current_token->token_type == TOKEN_TYPES::COMMA_T){
                advance();
                JsonNode value = *JsonValue();
                array.push_back(value);
                // advance();
                JsonArrayChildren(array);
            }
        }

        JsonNode* JsonArray(){
            if (current_token && current_token->token_type == TOKEN_TYPES::LSQBRACKET_T){
                advance();
                if (current_token){
                    std::vector<JsonNode> arr;
                    JsonNode *value = JsonValue();
                    if (value){
                        arr.push_back(*value);
                        JsonArrayChildren(arr);

                        if (current_token && current_token->token_type == TOKEN_TYPES::RSQBRACKET_T){
                            advance();
                            JsonNode* node = new JsonNode(arr);
                            allocated_memory.push_back(node);
                            return node;
                        }
                        else{
                            throw std::runtime_error("[ERROR] : INVALID JSON");
                        }
                    }  
                }
            }
            return NULL;

        }

        JsonNode* JsonObject(){
            if (current_token && current_token->token_type == TOKEN_TYPES::LBRACKET_T){
                advance();
                if (current_token){
                    std::unordered_map<std::string,JsonNode> map; 
                    // JsonNode* obj = new JsonNode(map);
                    // if (JsonProperty(*obj)){
                    if (JsonProperty(map)){
                        // JsonChildrenProperty(*obj);
                        JsonChildrenProperty(map);
                        if (current_token && current_token->token_type == TOKEN_TYPES::RBRACKET_T){
                            advance();
                            JsonNode* obj = new JsonNode(map);
                            allocated_memory.push_back(obj);
                            return obj;
                        }
                        else{
                            //missing } bracket
                            throw std::runtime_error("[ERROR] : INVALID JSON");
                        }
                    }
                }
            }
            return NULL;
        }

        bool isLiteral(JSON_PARSER::Token* token){
            if (token && token->token_type == TOKEN_TYPES::NUMBER_T || token->token_type == TOKEN_TYPES::STRING_T || token->token_type == TOKEN_TYPES::BOOLEAN_T || token->token_type == TOKEN_TYPES::NONE_T) return 1;
            return 0;
        }

        JsonNode* JsonValue(){
            if (current_token){
                JsonNode* obj = JsonObject();
                if (obj) return obj;

                JsonNode* arr = JsonArray();
                if (arr) return arr;

                if (isLiteral(current_token)){
                    std::string data = current_token->value;
                    if (current_token->token_type == TOKEN_TYPES::NUMBER_T){
                        // JsonNode* literal = new JsonNode(std::stoi(data));
                        JsonNode* literal = new JsonNode(std::stof(data));
                        allocated_memory.push_back(literal);
                        advance();
                        return literal;
                    }
                    else if (current_token->token_type == TOKEN_TYPES::STRING_T){
                        JsonNode* literal = new JsonNode(data);
                        allocated_memory.push_back(literal);
                        advance();
                        return literal;
                    }
                    else if (current_token->token_type == TOKEN_TYPES::BOOLEAN_T){
                        bool d = (data == "true");
                        JsonNode* literal = new JsonNode(d);
                        allocated_memory.push_back(literal);
                        advance();
                        return literal;
                    }
                    else{
                        //null
                        JsonNode* literal = new JsonNode();
                        allocated_memory.push_back(literal);
                        advance();
                        return literal;
                    }
                }
            }
            return NULL;
        }

        JsonNode* parse(){
            return JsonObject();
        }

        void dump(JsonNode* data,std::string file_path){
            // to put content in the file 
            // will write the json in the file
            Dumper d(data);
            d.dump(file_path);
        }

        void freeMemory(){
            for (int i = 0;i<allocated_memory.size();i++){
                // free(allocated_memory[i]);
                delete (JsonNode*)allocated_memory[i];
            }
        }

};

#endif