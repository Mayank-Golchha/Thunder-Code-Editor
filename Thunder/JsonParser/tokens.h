#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <variant>
#include <unordered_map>
#include <vector>
// #include <iostream>

// std::string TOKEN_TYPES_NAME[] = {"STRING","NUMBER","LBRACKET","RBRACKET","LSQBRACKET","RSQBRACKET","COMMA","COLON","BOOLEAN","NULL","ERROR"};
// int TOKEN_DATA_TYPES_LEN = 11;

// enum TOKEN_TYPES{
//     STRING,         // string
//     NUMBER,         // number
//     LBRACKET,       // {
//     RBRACKET,       // }
//     LSQBRACKET,     // [
//     RSQBRACKET,     // ]
//     COMMA,          // ,
//     COLON,          // :
//     BOOLEAN,        // true | false
//     NONE_,          // null
//     ERROR_          // error token  
// };

enum class JSON_TYPE{
    OBJECT, //key and value pairs
    ARRAY,
    STRING,
    NUMBER,
    BOOLEAN,
    NONE_
};

enum class TOKEN_TYPES{
    STRING_T,         // string
    NUMBER_T,         // number
    LBRACKET_T,       // {
    RBRACKET_T,       // }
    LSQBRACKET_T,     // [
    RSQBRACKET_T,     // ]
    COMMA_T,          // ,
    COLON_T,          // :
    BOOLEAN_T,        // true | false
    NONE_T,           // null
    ERROR_T           // error token  
};

namespace JSON_PARSER{
    struct Token{
        std::string value;
        TOKEN_TYPES token_type; 
    };
}

class JsonNode{
    public:
        //array -> std::vector<JsonNode>
        //property -> std::unordered_map<std::string,JsonNode>
        JSON_TYPE type;
        std::variant<std::vector<JsonNode>,std::unordered_map<std::string,JsonNode>,std::string,float,int,bool> data;
        
        JsonNode() : type(JSON_TYPE::NONE_){};

        JsonNode(const char* data_) : type(JSON_TYPE::STRING){
            data = data_;
        }

        JsonNode(const std::string &data_) : type(JSON_TYPE::STRING){
            data = data_;
        }

        JsonNode(float data_) : type(JSON_TYPE::NUMBER){
            data = data_;
        }

        JsonNode(int data_) : type(JSON_TYPE::NUMBER){
            data = data_;
        }

        JsonNode(bool data_) : type(JSON_TYPE::BOOLEAN){
            data = data_;
        }
         
        JsonNode(const std::vector<JsonNode> &data_) : type(JSON_TYPE::ARRAY){
            data = data_;
        }

        JsonNode(const std::unordered_map<std::string,JsonNode> &data_) : type(JSON_TYPE::OBJECT){
            data = data_;
        }

        //copy constructor
        JsonNode(const JsonNode &node){
            data = node.data;
            type = node.type;
        }

        JsonNode(JSON_TYPE t) : type(t){}

        //for imlicit conversion
        //now no need to use get method
        operator std::unordered_map<std::string,JsonNode>()const{
            return std::get<std::unordered_map<std::string,JsonNode>>(data);
        }

        operator std::vector<JsonNode>()const{
            return std::get<std::vector<JsonNode>>(data);
        }

        operator std::string()const{
            return std::get<std::string>(data);
        }

        operator int()const{
            return std::get<int>(data);
        }

        operator float()const{
            return std::get<float>(data);
        }

        operator bool()const{
            return std::get<bool>(data);
        }

        //copy operator
        JsonNode& operator=(const JsonNode &node){
            data = node.data;
            type = node.type;
            return *this;
        }

        bool isArray(){
            return type == JSON_TYPE::ARRAY;
        }

        bool isObject(){
            return type == JSON_TYPE::OBJECT;
        }

        JsonNode& operator[](int index){
            //to access element of array
            if (!isArray()){
                throw "[ERROR] : INVALID JSON";
            }
            if (index >= std::get<std::vector<JsonNode>>(data).size()){
                throw "[ERROR] : INVALID INDEX";
            }

            return std::get<std::vector<JsonNode>>(data)[index];
        }

        JsonNode& operator[](const char* key){
            if (!isObject()){
                throw "[ERROR] : INVALID JSON";
            }
            return std::get<std::unordered_map<std::string,JsonNode>>(data)[key];
        }

        JsonNode& operator[](const std::string& key){
            if (!isObject()){
                throw "[ERROR] : INVALID JSON";
            }
            return std::get<std::unordered_map<std::string,JsonNode>>(data)[key];

        }

        void append_array(const JsonNode& node){
            if (!isArray()){
                throw "[ERROR] : INVALID JSON";
            }
            std::get<std::vector<JsonNode>>(data).push_back(node);
        }

};

#endif