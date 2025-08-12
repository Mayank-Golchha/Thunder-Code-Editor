// maybe a bug because at some place the token may be skipped if it doesnt match condition

#ifndef C_PLUGIN_H
#define C_PLUGIN_H

#include <memory>
#include <iostream>

#include "c_keywords.h"
#include "c_operators.h"
#include "c_tokens.h"
#include "../plugin_tokens.h"
#include "c_colors.h"
// #include "python_autocompleter.h"
// #include "python_parser.h"


class CPlugin{
    public:
        std::string code;
        std::vector<std::unique_ptr<AutoCompleteToken>> tokens; //its items will not be present in another vector
        std::vector<std::shared_ptr<TokenStyle>> style_tokens; //its item will be present in display 
        std::vector<std::shared_ptr<TokenStyle>> display_tokens; //its item will be present in display 
        std::string current_string = "";
        std::string current_typed_word = "";
        // PythonBlock* ast = NULL;

        int cx = 0,cy = 0; //cursor positions
        C_Token* selected_token = NULL; //will be used by autocompleter -> the token where cursor is at

        std::wstring comment = L"//";

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

        void init(std::string code_){
            code = code_;
            index = -1;
            start_index = -1;
            line_no = 0;
            advance();
        }

        std::vector<C_Token*> load(std::string code_,int cursor_x_,int cursor_y_){
            current_string = "";
            current_typed_word = "";
            clear();
            cx = cursor_x_; //we want thing before cursor not after
            cy = cursor_y_;

            init(code_);
            std::vector<C_Token*> c_tokens = tokenize();
            // PythonParser parser(python_tokens,selected_token,cx,cy);
            // parser.parse();
            // ast = parser.global_block;
            // PythonAutoCompleter completer(parser.selected_block,selected_token);
            // tokens = completer.auto_complete_tokens();
            //maybe a bug
            // for (int i = 0;i<parser.tokens.size();i++){
                // delete parser.tokens[i];
            // }
            return c_tokens;
            // return python_tokens;
        }


        void advance(){
            if (index + 1 > code.size()){
                current_char = '\0';
                return;
            }
            index++;
            start_index++;
            current_char = code[index];
        }


        int match(std::string text,std::string word,int index_){
            //will match whether word in text after index
            if (index_ + word.size() >= text.size()) return 0;
            for (int i = index_;i<index_ + word.size();i++){
                if (text[i] != word[i-index_]) return 0;
            }
            return 1;
        }


        void display(int sx,int sy,int ex,int ey){
            //will return the style tokens that need to be displayed
            for (int i = 0;i<style_tokens.size();i++){
                if (style_tokens[i]->startLine >= sy && style_tokens[i]->endLine <=ey && style_tokens[i]->end >= sx && style_tokens[i]->start < ex) display_tokens.push_back(style_tokens[i]);
            }
        }

        void getCurrentToken(C_Token* tok,int sy,int ey,int sx,int ex){
            if (cx > sx && cx <= ex && cy >= sy && cy <= ey){
                selected_token = tok;
            }
        }

        C_Token* string_token(){
            //current token is "
            int s = index;
            int start = start_index;
            char c = current_char;
            int s_line = line_no;
            advance();
            // while (current_char && current_char != c) advance();
            while (current_char && current_char != c){
                if (current_char == '\n'){
                    start_index = -1;
                    line_no++;
                }
                advance();
            }
            if (!current_char){
                //maybe a bug here
                std::string word = code.substr(s+1,index-s);
                C_Token* token = new C_Token{word,word,C_TOKEN_TYPES::C_ERROR,s_line,line_no,start,start_index+1};
                style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_STRING_COLOR,s_line,line_no,start,start_index+1}));
                getCurrentToken(token,s_line,line_no,start,start_index+1);
                return token;
            }
            std::string word = code.substr(s+1,index-s-1);
            style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_STRING_COLOR,s_line,line_no,start,start_index+1}));
            C_Token* token = new C_Token{c + word + c,word,C_TOKEN_TYPES::STRING,s_line,line_no,start,start_index+1};
            advance();

            getCurrentToken(token,line_no,line_no,start,start_index+1);
            return token;
        }

        C_Token* char_token(){
            //current token is '
            int s = index;
            int start = start_index;
            char c = current_char;
            int s_line = line_no;
            advance();
            if (current_char){
                advance();
                if (current_char == '\''){
                    std::string word = code.substr(s,3);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_STRING_COLOR,s_line,line_no,start,start_index+1}));
                    C_Token* token = new C_Token{word,word,C_TOKEN_TYPES::CHAR,s_line,line_no,start,start_index+1};
                    advance();
                    return token;
                }
            }
            return NULL;
        }

        C_Token* number_token(){
            int s = index;
            int dot = 0;
            advance();

            while (current_char && dot < 2 &&  ((current_char >= '0' && current_char <= '9') || current_char == '.')){
                if (current_char == '.') dot++;
                advance();
            }
            std::string word = code.substr(s,index-s);
            C_Token* token = new C_Token{word,std::stof(word),C_TOKEN_TYPES::NUMBER,line_no,line_no,int(start_index-word.size()),start_index};
            style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_NUMBER_COLOR,line_no,line_no,int(start_index-word.size()),start_index}));

            getCurrentToken(token,line_no,line_no,int(start_index-word.size()),start_index);

            return token;
        }

        C_Token* Identifier(){
            //identifier are variable names,function names etc
            //current char has been checked
            int s = index;
            int start = start_index;
            if (current_char) advance();

            while (current_char && ((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z') || (current_char >= '0' && current_char <= '9') || current_char == '_')) {
                advance();
            }
            std::string word = code.substr(s,index-s);

            if (isKeyword(word)){
                C_Token* token = new C_Token{word,word,C_TOKEN_TYPES::KEYWORDS,line_no,line_no,start,start_index};
                style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_KEYWORD_COLOR,line_no,line_no,start,start_index}));
                getCurrentToken(token,line_no,line_no,start,start_index);
                return token;
            }
            C_Token* token = new C_Token{word,word,C_TOKEN_TYPES::IDENTIFIER,line_no,line_no,start,start_index};
            style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_IDENTIFIER_COLOR,line_no,line_no,start,start_index}));
            getCurrentToken(token,line_no,line_no,start,start_index);
            return token;
        }

        C_Token* preprocessor_directive(){
            int s = index;
            int start = start_index;
            for (int i = 0;i<C_PREPROCESSOR_DIRECTIVES.size();i++){
                if (match(code,C_PREPROCESSOR_DIRECTIVES[i],index)){
                    C_Token* token = new C_Token{C_PREPROCESSOR_DIRECTIVES[i],C_PREPROCESSOR_DIRECTIVES[i],C_TOKEN_TYPES::PREPROCESSOR_DIRECTIVES,line_no,line_no,start,start_index};
                    start_index += C_PREPROCESSOR_DIRECTIVES[i].size();
                    index += C_PREPROCESSOR_DIRECTIVES[i].size();
                    current_char = code[index];
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_PREPROCESSOR_COLOR,line_no,line_no,start,start_index}));
                    getCurrentToken(token,line_no,line_no,start,start_index);
                    return token;
                }
            }
            return NULL;
            // C_Token* token = new C_Token{word,word,C_TOKEN_TYPES::IDENTIFIER,line_no,line_no,start,start_index};
        }

        std::string strInFunc(std::string text,std::vector<std::string> words){
            //will check if any words in text
            for (std::string word :words){
                if (match(text,word,index)) return word;
            }
            return "";
        }

        int charInFunc(std::string text,char c){
            //will check if c is in text
            for (int i = 0;i<text.size();i++) if (text[i] == c) return 1;
            return 0;
        }

        C_Token* C_Operator(){
            int start = start_index;
            std::string word = strInFunc(code,c_comparison_operator);
            C_TOKEN_TYPES type = C_TOKEN_TYPES::COMPARISON_OPERATOR;
            if (word == ""){
                word = strInFunc(code,c_assignment_operator);
                type = C_TOKEN_TYPES::ASSIGNMENT_OPERATOR;
                if (word == ""){
                    word = strInFunc(code,c_bitwise_operator);
                    type = C_TOKEN_TYPES::BITWISE_OPERATOR;
                    if (word == ""){
                        word = strInFunc(code,c_arithmetic_operator);
                        type = C_TOKEN_TYPES::ARITHMETIC_OPERATOR;
                    }
                }

            }
            if (word != ""){
                C_Token* token = new C_Token{word,word,type,line_no,line_no,start,start_index+(int)word.size()};
                style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_OPERATOR_COLOR,line_no,line_no,start,start_index+(int)word.size()}));
                getCurrentToken(token,line_no,line_no,start,start_index);
                
                return token;
            }
            return NULL;
        }

        int isKeyword(std::string word){
            //will return whether given identifier is a keyword or not
            int low = 0,high = C_KEYWORDS.size()-1;

            int mid = 0;
            while (low <= high){
                mid = low + (high-low)/2;
                if (C_KEYWORDS[mid] == word) return 1;
                else if (C_KEYWORDS[mid] < word) low = mid+1;
                else high = mid-1;
            }

            return 0;
        }
       
        std::vector<C_Token*> tokenize(){
            std::vector<C_Token*> c_tokens;
            while (current_char){
                if (current_char == ' ' || current_char == '\t') advance();
                else if (current_char >= '0' && current_char <= '9') c_tokens.push_back(number_token());
                else if ((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z') || current_char == '_') c_tokens.push_back(Identifier());
                else if (current_char == '"' || current_char == '\'') c_tokens.push_back(string_token());
                else if (current_char == '#'){
                    C_Token* t = preprocessor_directive();
                    if (t) c_tokens.push_back(t);
                    else advance();
                }
                else if (current_char == '{'){
                    C_Token* token = new C_Token{"{","{",C_TOKEN_TYPES::LBRACKET,line_no,line_no,start_index,start_index+1};
                    c_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_BRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }
                else if (current_char == '}'){
                    // Python_Token* token = new Python_Token{"}","}",PYTHON_TOKEN_TYPES::R_BRACKET};
                    C_Token* token = new C_Token{"}","}",C_TOKEN_TYPES::RBRACKET,line_no,line_no,start_index,start_index+1};
                    c_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_BRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }
                else if (current_char == '('){
                    C_Token* token = new C_Token{"(","(",C_TOKEN_TYPES::LPARENTHESIS,line_no,line_no,start_index,start_index+1};
                    c_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_PARENTHESIS_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }
                else if (current_char == ')'){
                    C_Token* token = new C_Token{")",")",C_TOKEN_TYPES::RPARENTHESIS,line_no,line_no,start_index,start_index+1};
                    c_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_PARENTHESIS_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }
                else if (current_char == '['){
                    C_Token* token = new C_Token{"[","[",C_TOKEN_TYPES::LSQBRACKET,line_no,line_no,start_index,start_index+1};
                    c_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_SQBRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }
                else if (current_char == ']'){
                    C_Token* token = new C_Token{"]","]",C_TOKEN_TYPES::RSQBRACKET,line_no,line_no,start_index,start_index+1};
                    c_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_SQBRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }  
                else if (current_char == ','){
                    C_Token* token = new C_Token{",",",",C_TOKEN_TYPES::COMMA,line_no,line_no,start_index,start_index+1};
                    c_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_DELIMETER_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }  
                else if (current_char == ':'){
                    C_Token* token = new C_Token{":",":",C_TOKEN_TYPES::COLON,line_no,line_no,start_index,start_index+1};
                    c_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_DELIMETER_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }  
                else if (current_char == '\n'){
                    line_no++;
                    advance();
                    start_index = 0;
                } 
                else{
                    C_Token* token = C_Operator();
                    if (token){
                        index += token->raw_data.size();
                        start_index += token->raw_data.size();
                        if (index > code.size()) current_char = '\0';
                        else current_char = code[index];
                        c_tokens.push_back(token);

                        getCurrentToken(token,line_no,line_no,start_index,start_index+1); // 

                        continue;
                    }
                    std::string a = std::string(1,current_char);
                    C_Token* error_token = new C_Token{a,a,C_TOKEN_TYPES::C_ERROR,line_no,line_no,start_index,start_index+1};
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{C_ERROR_COLOR,line_no,line_no,start_index,start_index+1}));

                    c_tokens.push_back(error_token);
                    getCurrentToken(error_token,line_no,line_no,start_index,start_index+1);
                    advance();
                }

            }
            return c_tokens;
        }

        void clear(){
            //will free memory
            current_string = "";
            current_typed_word = "";
            selected_token = NULL;
            tokens.clear();
            style_tokens.clear();
            display_tokens.clear();
            cx = 0; cy = 0;
        }

    
    private:
        char current_char;
        int index = -1;
        int line_no = 0,start_index = -1;


};




#endif 