//TODO : in error token take all error tokens in one struct to reduce memory usage --> done but maybe a bug

#ifndef JSON_PLUGIN_H
#define JSON_PLUGIN_H

#include <memory>
#include <vector>
#include <string>

// #include "E:/Text Editor/Plugins/plugin_tokens.h" //bug here dont put actual path
#include "../plugin_tokens.h"
#include "json_colors.h"
#include <iostream>


class JsonPlugin{
    public:
        std::string code;
        int len;
        std::vector<std::unique_ptr<AutoCompleteToken>> tokens; //its items will not be present in another vector
        std::vector<std::shared_ptr<TokenStyle>> style_tokens; //its item will be present in display 
        std::vector<std::shared_ptr<TokenStyle>> display_tokens; //its item will be present in display 
        std::string current_string = "";
        std::string current_typed_word = ""; //the current word where cursor is  eg : hello -> hello        --> according to cursor position  
        // TokenStyle* current_style_token = NULL; //the current word part eg : hello it may be he if cursor is after e         --> according to cursor position

        std::wstring comment = L"";

        const std::vector<std::unique_ptr<AutoCompleteToken>>& getAutoCompleteTokens() const {
            //only have access to read
            return tokens;
        }

        const std::vector<std::shared_ptr<TokenStyle>>& getStyleTokens() const {
            return style_tokens;
        }

        const std::vector<std::shared_ptr<TokenStyle>>& getDisplayTokens() const {
            return display_tokens;
        }


        int match(std::string text,std::string word,int start){
            if (start + word.size() > len) return 0;
            return text.substr(start,word.size()) == word;   
        }

        void load(std::string code,int cursor_x,int cursor_y){
            //will split code
            current_typed_word = "";
            current_string = "";
            clear();
            len = code.size();
            
            int line_no = 0,start_index = 0,index = 0;
            while (index < len){
                if (code[index] == ' '){
                    start_index++;
                    index++;
                }
                else if (code[index] == '\n'){
                    start_index = 0;
                    index++;
                    line_no++;
                }
                else if (code[index] == '{' || code[index] == '}'){
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{JSON_BRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
                    index++;
                    start_index++;
                }
                else if (code[index] == '[' || code[index] == ']'){
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{JSON_SQBRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
                    index++;
                    start_index++;
                }
                else if (match(code,"true",index)){
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{JSON_BOOLEAN_COLOR,line_no,line_no,start_index,start_index+4}));
                    index += 4;
                    start_index += 4;
                }
                else if (match(code,"false",index)){
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{JSON_BOOLEAN_COLOR,line_no,line_no,start_index,start_index+5}));
                    index += 5;
                    start_index += 5;
                }
                else if (match(code,"null",index)){
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{JSON_BOOLEAN_COLOR,line_no,line_no,start_index,start_index+4}));
                    index += 4;
                    start_index += 4;
                }
                else if (code[index] == ',' || code[index] == ':'){
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{JSON_COMMA_COLON_COLOR,line_no,line_no,start_index,start_index+1}));
                    index++;
                    start_index++;
                }
                else if (code[index] >= '0' && code[index] <= '9'){
                    int s = index;
                    int dot = 0;
                    index++;
                    start_index++;
                    while (dot < 2 && ((code[index] >= '0' && code[index] <= '9') || code[index] == '.')){
                        if (code[index] == '.') dot++;
                        start_index++;
                        index++;
                    }
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{JSON_NUMBER_COLOR,line_no,line_no,start_index-(index-s),start_index}));

                }
                else if (code[index] == '"'){
                    int s = index;
                    index++;
                    start_index++;
                    while (code[index] && code[index] != '"') {
                        if (code[index] == '\n'){
                            index--;
                            start_index--;
                            break;
                        }
                        index++;
                        start_index++;
                    }
                    index++;
                    start_index++;
                    COLORREF color = JSON_STRING_COLOR;
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{color,line_no,line_no,start_index-(index-s),start_index}));
                }
                else{
                    int s = start_index;
                    // index++;
                    // start_index++;
                    // while (index < len && ((code[index] >= 'a' && code[index] <= 'z') || (code[index] >= 'A' && code[index] <= 'Z'))){
                    while (index < len && code[index] != '"' && (code[index] < '0' || code[index] > '9') && code[index] != '\n' && code[index] != ' '){
                        index++;
                        start_index++;
                        if (code[index] == '\n'){
                            start_index--;
                            index--;
                            break;
                        }
                    }
                    // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{JSON_ERROR_COLOR,line_no,line_no,start_index,start_index+1}));
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{JSON_ERROR_COLOR,line_no,line_no,s,start_index+1}));
                    index++;
                    start_index++;
                }
            }   
        }

    void clear(){
        //will free memory
        current_string = "";
        current_typed_word = "";
        tokens.clear();
        style_tokens.clear();
        display_tokens.clear();
    }
    
    void display(int sx,int sy,int ex,int ey){
        //will return the style tokens that need to be displayed
        for (int i = 0;i<style_tokens.size();i++){
            if (style_tokens[i]->startLine >= sy && style_tokens[i]->endLine <=ey && style_tokens[i]->end >= sx && style_tokens[i]->start < ex) display_tokens.push_back(style_tokens[i]);
        }
    }

};


#endif