// #ifndef DUMPER_H
// #define DUMPER_H

// #include <fstream>
// #include <stack>

// #include "tokens.h"


// class Dumper{
//     public:
//         JsonNode* json;
//         std::stack<int> indentations;

//         Dumper(JsonNode* json_){
//             json = json_;
//             indentations.push(0);
//         }

//         void tab_spaces(std::string& content){
//             std::string spaces(indentations.top()*3,' ');
//             content += spaces;
//         }

//         void traverse(JsonNode node,std::string& content){
//             if (node.type == JSON_TYPE::NUMBER){
//                 content += std::to_string(float(node));
//             }
//             else if (node.type == JSON_TYPE::STRING){
//                 content += "\"" + std::string(node) + "\"";
//             }
//             else if (node.type == JSON_TYPE::BOOLEAN){
//                 if (bool(node)) content += "true";
//                 else content += "false";
//             }
//             else if (node.type == JSON_TYPE::NONE_){
//                 content += "null";
//             }
//             else if (node.type == JSON_TYPE::ARRAY){
//                 std::vector<JsonNode> array = std::vector<JsonNode>(node);
                
//                 content += "[";
//                 traverse(array[0],content);
//                 for (int i = 1;i < array.size();i++){
//                     content += ", ";
//                     traverse(array[i],content);
//                 }
//                 content += "]";
//             }
//             else if (node.type == JSON_TYPE::OBJECT){
//                 std::unordered_map<std::string,JsonNode> object = std::unordered_map<std::string,JsonNode>(node);
//                 tab_spaces(content);
//                 content += "{\n";
//                 int top = indentations.top();
//                 indentations.push(1 + top);

//                 for (auto item : object){
//                     tab_spaces(content);
//                     content += "\"" + std::string(item.first) + "\" : ";
//                     traverse(item.second,content);
//                     content += ",\n";
//                 }
//                 content.pop_back();
//                 indentations.pop();
//                 tab_spaces(content);
//                 content += "\n}\n";
//             }

//             // indentations.pop();
//             content += "\n";
//             tab_spaces(content);
//             content += "}";
            
//         }

//         void dump(std::string file_path){
//             std::ofstream file(file_path);
//             std::string content;
//             std::cout << "helo";
//             traverse(*json,content);
//             std::cout << "world";
//             file << content;
//         }
// };


// #endif






#ifndef DUMPER_H
#define DUMPER_H

#include <fstream>
#include <stack>

#include "tokens.h"


class Dumper{
    public:
        JsonNode* json;
        std::stack<int> indentations;

        Dumper(JsonNode* json_){
            json = json_;
            indentations.push(0);
        }

        void tab_spaces(std::string& content){
            std::string spaces(indentations.top()*3,' ');
            content += spaces;
        }

        void traverse(JsonNode* node,std::string& content){
            if (node->type == JSON_TYPE::NUMBER){
                content += std::to_string(float(*node));
            }
            else if (node->type == JSON_TYPE::STRING){
                content += "\"" + std::string(*node) + "\"";
            }
            else if (node->type == JSON_TYPE::BOOLEAN){
                if (bool(*node)) content += "true";
                else content += "false";
            }
            else if (node->type == JSON_TYPE::NONE_){
                content += "null";
            }
            else if (node->type == JSON_TYPE::ARRAY){
                std::vector<JsonNode> array = std::vector<JsonNode>(*node);
                content += "[";
                traverse(&array[0],content);
                for (int i = 1;i < array.size();i++){
                    content += ", ";
                    traverse(&array[i],content);
                }
                content += "]";
            }
            else if (node->type == JSON_TYPE::OBJECT){
                std::unordered_map<std::string,JsonNode> object = std::unordered_map<std::string,JsonNode>(*node);
                tab_spaces(content);
                content += "{\n";
                int top = indentations.top();
                indentations.push(1 + top);
                for (auto& item : object){
                    tab_spaces(content);
                    content += "\"" + std::string(item.first) + "\" : ";
                    traverse(&item.second,content);
                    content += ",\n";
                }
                content.pop_back();
                indentations.pop();
                tab_spaces(content);
                content += "\n}\n";
            }
        }

        void deleteJsonNode(JsonNode* node) {
            if (!node) return;

            switch (node->type) {
                case JSON_TYPE::ARRAY: {
                    auto& array = std::get<std::vector<JsonNode>>(node->data);
                    for (auto& item : array)
                        deleteJsonNode(&item);
                    break;
                }
                case JSON_TYPE::OBJECT: {
                    auto& object = std::get<std::unordered_map<std::string, JsonNode>>(node->data);
                    for (auto& [_, value] : object)
                        deleteJsonNode(&value);
                    break;
                }
                default: break;
            }

            delete node;
        }


        void dump(std::string file_path){
            // to write json in file
            std::ofstream file(file_path);
            std::string content;
            traverse(json,content);
            file << content;
            file.close();
        }
        ~Dumper(){
            // deleteJsonNode(json);

        }
};


#endif