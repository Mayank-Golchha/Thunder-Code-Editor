// //maybe a bug in string token because of "|'

// #ifndef PYTHON_PLUGIN_H
// #define PYTHON_PLUGIN_H

// #include <memory>
// #include <iostream>

// #include "python_keywords.h"
// #include "python_operators.h"
// #include "../plugin_tokens.h"
// #include "python_colors.h"
// #include "python_autocompleter.h"
// #include "python_parser.h"


// class PythonPlugin{
//     public:
//         std::string code;
//         std::vector<std::unique_ptr<AutoCompleteToken>> tokens; //its items will not be present in another vector
//         std::vector<std::shared_ptr<TokenStyle>> style_tokens; //its item will be present in display 
//         std::vector<std::shared_ptr<TokenStyle>> display_tokens; //its item will be present in display 
//         std::string current_string = "";
//         std::string current_typed_word = "";
//         PythonBlock* ast = NULL;

//         int cx = 0,cy = 0; //cursor positions
//         Python_Token* selected_token = NULL; //will be used by autocompleter -> the token where cursor is at

//         const std::vector<std::unique_ptr<AutoCompleteToken>>& getAutoCompleteTokens() const {
//             //only have access to read
//             return tokens;
//         }

//         const std::vector<std::shared_ptr<TokenStyle>>& getStyleTokens() const {
//             return style_tokens;
//         }

//         const std::vector<std::shared_ptr<TokenStyle>>& getDisplayTokens() const {
//             return display_tokens;
//         }


//         void advance(){
//             if (index + 1 > code.size()){
//                 current_char = '\0';
//                 return;
//             }
//             index++;
//             start_index++;
//             current_char = code[index];
//         }

//         void init(std::string code_){
//             code = code_;
//             index = -1;
//             start_index = -1;
//             line_no = 0;
//             advance();
//         }

//         int match(std::string text,std::string word,int index_){
//             //will match whether word in text after index
//             if (index_ + word.size() >= text.size()) return 0;
//             for (int i = index_;i<index_ + word.size();i++){
//                 if (text[i] != word[i-index_]) return 0;
//             }
//             return 1;
//         }

//         // void load(std::string code_,int cursor_x,int cursor_y){
//         std::vector<Python_Token*> load(std::string code_,int cursor_x_,int cursor_y_){
//             clear();
//             cx = cursor_x_; //we want thing before cursor not after
//             cy = cursor_y_;

//             init(code_);
//             std::vector<Python_Token*> python_tokens = tokenize();
//             PythonParser parser(python_tokens,selected_token);
//             parser.parse();
//             ast = parser.global_block;
//             PythonAutoCompleter completer(parser.selected_block,selected_token);

//             tokens = completer.auto_complete_tokens();
//             return python_tokens;
//         }

//         // void display(int sy,int ey,int sx,int ex){
//         void display(int sx,int sy,int ex,int ey){
//             //will return the style tokens that need to be displayed
//             for (int i = 0;i<style_tokens.size();i++){
//                 if (style_tokens[i]->startLine >= sy && style_tokens[i]->endLine <=ey && style_tokens[i]->end >= sx && style_tokens[i]->start < ex) display_tokens.push_back(style_tokens[i]);
//             }
//         }

//         void getCurrentToken(Python_Token* tok,int sy,int ey,int sx,int ex){
//             if (cx > sx && cx <= ex && cy >= sy && cy <= ey){
//                 selected_token = tok;
//             }
//         }

//         Python_Token* string_token(){
//             //current token is ' or "
//             int s = index;
//             int start = start_index;
//             char c = current_char;
//             advance();
//             while (current_char && current_char != c) advance();
//             if (!current_char){
//                 std::string word = code.substr(s+1,index-s-1);
//                 Python_Token* token = new Python_Token{word,word,PYTHON_TOKEN_TYPES::PYTHON_ERROR};
//                 return token;
//             }
//             std::string word = code.substr(s+1,index-s-1);
//             // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_STRING_COLOR,line_no,line_no,int(start_index+word.size()-2),start_index+1}));
//             style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_STRING_COLOR,line_no,line_no,start,start_index+1}));
//             advance();

//             Python_Token* token = new Python_Token{c + word + c,word,PYTHON_TOKEN_TYPES::STRING};

//             getCurrentToken(token,line_no,line_no,start,start_index+1);
//             return token;
//         }

//         Python_Token* number_token(){
//             int s = index;
//             int dot = 0;
//             advance();

//             while (current_char && dot < 2 &&  ((current_char >= '0' && current_char <= '9') || current_char == '.')){
//                 if (current_char == '.') dot++;
//                 advance();
//             }
//             std::string word = code.substr(s,index-s);
//             Python_Token* token = new Python_Token{word,std::stof(word),PYTHON_TOKEN_TYPES::NUMBER};
//             style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_NUMBER_COLOR,line_no,line_no,int(start_index-word.size()),start_index}));

//             getCurrentToken(token,line_no,line_no,int(start_index-word.size()),start_index);

//             return token;
//         }

//         Python_Token* Identifier(){
//             //identifier are variable names,function names etc
//             //current char has been checked
//             int s = index;
//             int start = start_index;
//             if (current_char)
//             advance();

//             while (current_char && ((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z') || (current_char >= '0' && current_char <= '9') || current_char == '_')) {
//                 advance();
//             }
//             std::string word = code.substr(s,index-s);

//             if (isKeyword(word)){
//                 Python_Token* token = new Python_Token{word,word,PYTHON_TOKEN_TYPES::KEYWORD};
//                 style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_KEYWORD_COLOR,line_no,line_no,start,start_index}));
//                 getCurrentToken(token,line_no,line_no,start,start_index);
//                 return token;
//             }
//             Python_Token* token = new Python_Token{word,word,PYTHON_TOKEN_TYPES::IDENTIFIER};
//             style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_IDENTIFIER_COLOR,line_no,line_no,start,start_index}));
//             getCurrentToken(token,line_no,line_no,start,start_index);
//             return token;
//         }

//         std::string strInFunc(std::string text,std::vector<std::string> words){
//             //will check if any words in text
//             for (std::string word :words){
//                 if (match(text,word,index)) return word;
//             }
//             return "";
//         }

//         int charInFunc(std::string text,char c){
//             //will check if c is in text
//             for (int i = 0;i<text.size();i++) if (text[i] == c) return 1;
//             return 0;
//         }

//         Python_Token* Python_Operator(){
//             int start = start_index;
//             std::string word = strInFunc(code,python_comparison_operator);
//             PYTHON_TOKEN_TYPES type = PYTHON_TOKEN_TYPES::COMPARISON_OPERATOR;
//             if (word == ""){
//                 word = strInFunc(code,python_assignment_operator);
//                 type = PYTHON_TOKEN_TYPES::ASSIGNMENT_OPERATOR;
//                 if (word == ""){
//                     word = strInFunc(code,python_bitwise_operator);
//                     type = PYTHON_TOKEN_TYPES::BITWISE_OPERATOR;
//                     if (word == ""){
//                         word = strInFunc(code,python_arithmetic_operator);
//                         type = PYTHON_TOKEN_TYPES::ARITHMETIC_OPERATOR;
//                     }

//                 }


//             }
//             if (word != ""){
//                 // Python_Token* token = new Python_Token{word,word,PYTHON_TOKEN_TYPES::OPERATOR};
//                 Python_Token* token = new Python_Token{word,word,type};
//                 style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_OPERATOR_COLOR,line_no,line_no,start,start_index}));
//                 getCurrentToken(token,line_no,line_no,start,start_index);
                
//                 return token;
//             }
//             return NULL;
//         }

//         int isKeyword(std::string word){
//             //will return whether given identifier is a keyword or not
//             int low = 0,high = PYTHON_KEYOWORDS.size()-1;

//             int mid = 0;
//             while (low <= high){
//                 mid = low + (high-low)/2;
//                 if (PYTHON_KEYOWORDS[mid] == word) return 1;
//                 // else if (PYTHON_KEYOWORDS[mid] > word) low = mid+1;
//                 else if (PYTHON_KEYOWORDS[mid] < word) low = mid+1;
//                 else high = mid-1;
//             }

//             return 0;
//         }
       
//         std::vector<Python_Token*> tokenize(){
//             std::vector<Python_Token*> python_tokens;
//             while (current_char){
//                 if (current_char >= '0' && current_char <= '9') python_tokens.push_back(number_token());
//                 // if (current_char >= '0' && current_char <= '9') number_token();
//                 else if ((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z') || current_char == '_') python_tokens.push_back(Identifier());
//                 // else if ((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z') || current_char == '_') Identifier();
//                 else if (current_char == '"' || current_char == '\'') python_tokens.push_back(string_token());
//                 // else if (current_char == '"' || current_char == '\'') string_token();
//                 else if (current_char == '{'){
//                     Python_Token* token = new Python_Token{"{","{",PYTHON_TOKEN_TYPES::L_BRACKET};
//                     python_tokens.push_back(token);
//                     style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_BRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
//                     getCurrentToken(token,line_no,line_no,start_index,start_index+1);
//                     advance();
//                 }
//                 else if (current_char == '}'){
//                     // Python_Token* token = new Python_Token{"}","}",PYTHON_TOKEN_TYPES::R_BRACKET};
//                     Python_Token* token = new Python_Token{"}","}",PYTHON_TOKEN_TYPES::R_BRACKET};
//                     python_tokens.push_back(token);
//                     style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_BRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
//                     getCurrentToken(token,line_no,line_no,start_index,start_index+1);
//                     advance();
//                 }
//                 else if (current_char == '('){
//                     Python_Token* token = new Python_Token{"(","(",PYTHON_TOKEN_TYPES::L_PARENTHESIS};
//                     python_tokens.push_back(token);
//                     style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_PARENTHESIS_COLOR,line_no,line_no,start_index,start_index+1}));
//                     getCurrentToken(token,line_no,line_no,start_index,start_index+1);
//                     advance();
//                 }
//                 else if (current_char == ')'){
//                     Python_Token* token = new Python_Token{")",")",PYTHON_TOKEN_TYPES::R_PARENTHESIS};
//                     python_tokens.push_back(token);
//                     style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_PARENTHESIS_COLOR,line_no,line_no,start_index,start_index+1}));
//                     getCurrentToken(token,line_no,line_no,start_index,start_index+1);
//                     advance();
//                 }
//                 else if (current_char == '['){
//                     Python_Token* token = new Python_Token{"[","[",PYTHON_TOKEN_TYPES::LSQ_BRACKET};
//                     python_tokens.push_back(token);
//                     style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_SQBRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
//                     getCurrentToken(token,line_no,line_no,start_index,start_index+1);
//                     advance();
//                 }
//                 else if (current_char == ']'){
//                     Python_Token* token = new Python_Token{"]","]",PYTHON_TOKEN_TYPES::RSQ_BRACKET};
//                     python_tokens.push_back(token);
//                     style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_SQBRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
//                     getCurrentToken(token,line_no,line_no,start_index,start_index+1);
//                     advance();
//                 }  
//                 else if (current_char == ','){
//                     Python_Token* token = new Python_Token{",",",",PYTHON_TOKEN_TYPES::COMMA};
//                     python_tokens.push_back(token);
//                     style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_DELIMETER_COLOR,line_no,line_no,start_index,start_index+1}));
//                     getCurrentToken(token,line_no,line_no,start_index,start_index+1);
//                     advance();
//                 }  
//                 else if (current_char == ':'){
//                     Python_Token* token = new Python_Token{":",":",PYTHON_TOKEN_TYPES::COLON};
//                     python_tokens.push_back(token);
//                     style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_DELIMETER_COLOR,line_no,line_no,start_index,start_index+1}));
//                     getCurrentToken(token,line_no,line_no,start_index,start_index+1);
//                     advance();
//                 }  
//                 else if (current_char == '\n'){
//                     Python_Token* token = new Python_Token{"\n","\n",PYTHON_TOKEN_TYPES::NEWLINE};
//                     python_tokens.push_back(token);
//                     // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_COMMA_COLON_COLOR,line_no,line_no,start_index,start_index+1}));
//                     line_no++;
//                     advance();
//                     start_index = 0;
//                 } 
//                 else if (current_char == '\t'){
//                     Python_Token* token = new Python_Token{"\t","\t",PYTHON_TOKEN_TYPES::INDENT};
//                     python_tokens.push_back(token);
//                     // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_COMMA_COLON_COLOR,line_no,line_no,start_index,start_index+1}));
//                     advance();
//                 }
//                 else if (current_char == ' '){
//                     Python_Token* token = new Python_Token{" "," ",PYTHON_TOKEN_TYPES::INDENT};
//                     // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_COMMA_COLON_COLOR,line_no,line_no,start_index,start_index+1}));
//                     python_tokens.push_back(token);
//                     advance();
//                 }
//                 else{
//                     Python_Token* token = Python_Operator();
//                     if (token){
//                         index += token->raw_data.size();
//                         start_index += token->raw_data.size();
//                         if (index > code.size()) current_char = '\0';
//                         else current_char = code[index];
//                         python_tokens.push_back(token);
//                         getCurrentToken(token,line_no,line_no,start_index,start_index+1);
//                         // advance();
//                         // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_COMMA_COLON_COLOR,line_no,line_no,start_index-1,start_index}));

//                         continue;
//                     }
//                     std::string a = std::string(1,current_char);
//                     Python_Token* error_token = new Python_Token{a,a,PYTHON_TOKEN_TYPES::PYTHON_ERROR};
//                     // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_ERROR_COLOR,line_no,line_no,start_index-1,start_index}));
//                     style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_ERROR_COLOR,line_no,line_no,start_index,start_index+1}));

//                     python_tokens.push_back(error_token);
//                     getCurrentToken(error_token,line_no,line_no,start_index,start_index+1);
//                     advance();
//                 }

//             }
//             return python_tokens;
//         }

//         void clear(){
//             //will free memory
//             current_string = "";
//             current_typed_word = "";
//             selected_token = NULL;
//             tokens.clear();
//             style_tokens.clear();
//             display_tokens.clear();
//             cx = 0; cy = 0;
//         }
    
//     private:
//         char current_char;
//         int index = -1;
//         int line_no = 0,start_index = -1;

// };

// #endif

























//maybe a bug in string token because of "|'

#ifndef PYTHON_PLUGIN_H
#define PYTHON_PLUGIN_H

#include <memory>
#include <iostream>

#include "python_keywords.h"
#include "python_operators.h"
#include "../plugin_tokens.h"
#include "python_colors.h"
#include "python_autocompleter.h"
#include "python_parser.h"


class PythonPlugin{
    public:
        std::string code;
        std::vector<std::unique_ptr<AutoCompleteToken>> tokens; //its items will not be present in another vector
        std::vector<std::shared_ptr<TokenStyle>> style_tokens; //its item will be present in display 
        std::vector<std::shared_ptr<TokenStyle>> display_tokens; //its item will be present in display 
        std::string current_string = "";
        std::string current_typed_word = "";
        PythonBlock* ast = NULL;

        std::wstring comment = L"#";


        int cx = 0,cy = 0; //cursor positions
        Python_Token* selected_token = NULL; //will be used by autocompleter -> the token where cursor is at

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


        void advance(){
            if (index + 1 > code.size()){
                current_char = '\0';
                return;
            }
            index++;
            start_index++;
            current_char = code[index];
            // if (index + 1 < code.size()){
                // index++;
                // start_index++;
                // current_char = code[index];
                // return ;
            // }
            // current_char = '\0';
        }

        void init(std::string code_){
            code = code_;
            index = -1;
            start_index = -1;
            line_no = 0;
            advance();
        }

        int match(std::string text,std::string word,int index_){
            //will match whether word in text after index
            if (index_ + word.size() >= text.size()) return 0;
            for (int i = index_;i<index_ + word.size();i++){
                if (text[i] != word[i-index_]) return 0;
            }
            return 1;
        }

        // Python_Token* getCurrentToken(int cursor_x_,int cursor_y_){
            // cx = cursor_x_; //we want thing before cursor not after
            // cy = cursor_y_;
            // for (int i = 0;i<tokens.size();i++){
                // if (cx > tokens[i].star && cx <= ex && cy >= sy && cy <= ey){
                //    selected_token = tok;
                // }
            // }

        // }

        std::vector<Python_Token*> load(std::string code_,int cursor_x_,int cursor_y_){
        // std::vector<Python_Token*> load(std::string code_){
            current_string = "";
            current_typed_word = "";
            clear();
            cx = cursor_x_; //we want thing before cursor not after
            cy = cursor_y_;

            init(code_);
            std::vector<Python_Token*> python_tokens = tokenize();
            
            PythonParser parser(python_tokens,selected_token,cx,cy);
            parser.parse();
            ast = parser.global_block;
            // selected_token = parser.getCurrentToken(cx,cy);
            
            PythonAutoCompleter completer(parser.selected_block,selected_token);

            tokens = completer.auto_complete_tokens();

            if (selected_token) current_typed_word = parser.selected_token->raw_data;
            // current_typed_word = parser.selected_token->raw_data;
            //maybe a bug
            for (int i = 0;i<parser.tokens.size();i++){
                delete parser.tokens[i];
            }
            return python_tokens;
        }

        // void display(int sy,int ey,int sx,int ex){
        void display(int sx,int sy,int ex,int ey){
            //will return the style tokens that need to be displayed
            for (int i = 0;i<style_tokens.size();i++){
                std::cout << style_tokens[i]->start << " " << style_tokens[i]->end << " " << style_tokens[i]->startLine << " " << style_tokens[i]->endLine << "\n";
                if (style_tokens[i]->startLine >= sy && style_tokens[i]->endLine <=ey && style_tokens[i]->end >= sx && style_tokens[i]->start < ex) display_tokens.push_back(style_tokens[i]);
                // else if (sy >= style_tokens[i]->startLine && ey < style_tokens[i]->startLine)
            }
        }

        void getCurrentToken(Python_Token* tok,int sy,int ey,int sx,int ex){
            std::cout << "hel0 " << cx << " " << cy << " " << sx << " " << ex << "\n" ;
            if (cx > sx && cx <= ex && cy >= sy && cy <= ey){
                selected_token = tok;
                std::cout << "world";
            }
        }

        Python_Token* string_token(){
            //current token is ' or "
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
                // Python_Token* token = new Python_Token{word,word,PYTHON_TOKEN_TYPES::PYTHON_ERROR,line_no,line_no,start,start_index+1};
                // Python_Token* token = new Python_Token{word,word,PYTHON_TOKEN_TYPES::PYTHON_ERROR,line_no,line_no,start,start_index+1};
                Python_Token* token = new Python_Token{word,word,PYTHON_TOKEN_TYPES::PYTHON_ERROR,s_line,line_no,start,start_index+1};
                // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_STRING_COLOR,line_no,line_no,start,start_index+1}));
                style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_STRING_COLOR,s_line,line_no,start,start_index+1}));
                // getCurrentToken(token,line_no,line_no,start,start_index+1);
                getCurrentToken(token,s_line,line_no,start,start_index+1);
                return token;
            }
            std::string word = code.substr(s+1,index-s-1);

            // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_STRING_COLOR,line_no,line_no,int(start_index+word.size()-2),start_index+1}));
            // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_STRING_COLOR,line_no,line_no,start,start_index+1}));
            style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_STRING_COLOR,s_line,line_no,start,start_index+1}));
            // advance();

            // Python_Token* token = new Python_Token{c + word + c,word,PYTHON_TOKEN_TYPES::STRING,line_no,line_no,start,start_index+1};
            Python_Token* token = new Python_Token{c + word + c,word,PYTHON_TOKEN_TYPES::STRING,s_line,line_no,start,start_index+1};
            advance();

            getCurrentToken(token,line_no,line_no,start,start_index+1);
            return token;
        }

        Python_Token* number_token(){
            int s = index;
            int dot = 0;
            advance();

            while (current_char && dot < 2 &&  ((current_char >= '0' && current_char <= '9') || current_char == '.')){
                if (current_char == '.') dot++;
                advance();
            }
            std::string word = code.substr(s,index-s);
            Python_Token* token = new Python_Token{word,std::stof(word),PYTHON_TOKEN_TYPES::NUMBER,line_no,line_no,int(start_index-word.size()),start_index};
            style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_NUMBER_COLOR,line_no,line_no,int(start_index-word.size()),start_index}));

            getCurrentToken(token,line_no,line_no,int(start_index-word.size()),start_index);

            return token;
        }

        Python_Token* Identifier(){
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
                Python_Token* token = new Python_Token{word,word,PYTHON_TOKEN_TYPES::KEYWORD,line_no,line_no,start,start_index};
                // Python_Token* token = new Python_Token{word,word,PYTHON_TOKEN_TYPES::KEYWORD,line_no,line_no,start,start_index+1};
                style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_KEYWORD_COLOR,line_no,line_no,start,start_index}));
                // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_KEYWORD_COLOR,line_no,line_no,start,start_index+1}));
                getCurrentToken(token,line_no,line_no,start,start_index);
                return token;
            }
            Python_Token* token = new Python_Token{word,word,PYTHON_TOKEN_TYPES::IDENTIFIER,line_no,line_no,start,start_index};
            // Python_Token* token = new Python_Token{word,word,PYTHON_TOKEN_TYPES::IDENTIFIER,line_no,line_no,start,start_index+1};
            style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_IDENTIFIER_COLOR,line_no,line_no,start,start_index}));
            // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_IDENTIFIER_COLOR,line_no,line_no,start,start_index+1}));
            getCurrentToken(token,line_no,line_no,start,start_index);
            return token;
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

        Python_Token* Python_Operator(){
            int start = start_index;
            std::string word = strInFunc(code,python_comparison_operator);
            PYTHON_TOKEN_TYPES type = PYTHON_TOKEN_TYPES::COMPARISON_OPERATOR;
            if (word == ""){
                word = strInFunc(code,python_assignment_operator);
                type = PYTHON_TOKEN_TYPES::ASSIGNMENT_OPERATOR;
                if (word == ""){
                    word = strInFunc(code,python_bitwise_operator);
                    type = PYTHON_TOKEN_TYPES::BITWISE_OPERATOR;
                    if (word == ""){
                        word = strInFunc(code,python_arithmetic_operator);
                        type = PYTHON_TOKEN_TYPES::ARITHMETIC_OPERATOR;
                    }

                }


            }
            if (word != ""){
                // Python_Token* token = new Python_Token{word,word,PYTHON_TOKEN_TYPES::OPERATOR};
                Python_Token* token = new Python_Token{word,word,type,line_no,line_no,start,start_index+(int)word.size()};
                style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_OPERATOR_COLOR,line_no,line_no,start,start_index+(int)word.size()}));
                getCurrentToken(token,line_no,line_no,start,start_index);
                
                return token;
            }
            return NULL;
        }

        int isKeyword(std::string word){
            //will return whether given identifier is a keyword or not
            int low = 0,high = PYTHON_KEYOWORDS.size()-1;

            int mid = 0;
            while (low <= high){
                mid = low + (high-low)/2;
                if (PYTHON_KEYOWORDS[mid] == word) return 1;
                // else if (PYTHON_KEYOWORDS[mid] > word) low = mid+1;
                else if (PYTHON_KEYOWORDS[mid] < word) low = mid+1;
                else high = mid-1;
            }

            return 0;
        }
       
        std::vector<Python_Token*> tokenize(){
            std::vector<Python_Token*> python_tokens;
            while (current_char){
                if (current_char >= '0' && current_char <= '9') python_tokens.push_back(number_token());
                // if (current_char >= '0' && current_char <= '9') number_token();
                else if ((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z') || current_char == '_') python_tokens.push_back(Identifier());
                // else if ((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z') || current_char == '_') Identifier();
                else if (current_char == '"' || current_char == '\'') python_tokens.push_back(string_token());
                // else if (current_char == '"' || current_char == '\'') string_token();
                else if (current_char == '{'){
                    Python_Token* token = new Python_Token{"{","{",PYTHON_TOKEN_TYPES::L_BRACKET,line_no,line_no,start_index,start_index+1};
                    python_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_BRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }
                else if (current_char == '}'){
                    // Python_Token* token = new Python_Token{"}","}",PYTHON_TOKEN_TYPES::R_BRACKET};
                    Python_Token* token = new Python_Token{"}","}",PYTHON_TOKEN_TYPES::R_BRACKET,line_no,line_no,start_index,start_index+1};
                    python_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_BRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }
                else if (current_char == '('){
                    Python_Token* token = new Python_Token{"(","(",PYTHON_TOKEN_TYPES::L_PARENTHESIS,line_no,line_no,start_index,start_index+1};
                    python_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_PARENTHESIS_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }
                else if (current_char == ')'){
                    Python_Token* token = new Python_Token{")",")",PYTHON_TOKEN_TYPES::R_PARENTHESIS,line_no,line_no,start_index,start_index+1};
                    python_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_PARENTHESIS_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }
                else if (current_char == '['){
                    Python_Token* token = new Python_Token{"[","[",PYTHON_TOKEN_TYPES::LSQ_BRACKET,line_no,line_no,start_index,start_index+1};
                    python_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_SQBRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }
                else if (current_char == ']'){
                    Python_Token* token = new Python_Token{"]","]",PYTHON_TOKEN_TYPES::RSQ_BRACKET,line_no,line_no,start_index,start_index+1};
                    python_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_SQBRACKET_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }  
                else if (current_char == ','){
                    Python_Token* token = new Python_Token{",",",",PYTHON_TOKEN_TYPES::COMMA,line_no,line_no,start_index,start_index+1};
                    python_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_DELIMETER_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }  
                else if (current_char == ':'){
                    Python_Token* token = new Python_Token{":",":",PYTHON_TOKEN_TYPES::COLON,line_no,line_no,start_index,start_index+1};
                    python_tokens.push_back(token);
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_DELIMETER_COLOR,line_no,line_no,start_index,start_index+1}));
                    getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                    advance();
                }  
                else if (current_char == '\n'){
                    Python_Token* token = new Python_Token{"\n","\n",PYTHON_TOKEN_TYPES::NEWLINE,line_no,line_no,start_index,start_index+1};
                    python_tokens.push_back(token);
                    // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_COMMA_COLON_COLOR,line_no,line_no,start_index,start_index+1}));
                    line_no++;
                    advance();
                    start_index = 0;
                } 
                else if (current_char == '\t'){
                    Python_Token* token = new Python_Token{"\t","\t",PYTHON_TOKEN_TYPES::INDENT,line_no,line_no,start_index,start_index+1};
                    python_tokens.push_back(token);
                    // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_COMMA_COLON_COLOR,line_no,line_no,start_index,start_index+1}));
                    advance();
                }
                else if (current_char == ' '){
                    Python_Token* token = new Python_Token{" "," ",PYTHON_TOKEN_TYPES::INDENT,line_no,line_no,start_index,start_index+1};
                    // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_COMMA_COLON_COLOR,line_no,line_no,start_index,start_index+1}));
                    python_tokens.push_back(token);
                    advance();
                }
                else{
                    Python_Token* token = Python_Operator();
                    if (token){
                        index += token->raw_data.size();
                        start_index += token->raw_data.size();
                        if (index > code.size()) current_char = '\0';
                        else current_char = code[index];
                        python_tokens.push_back(token);
                        // getCurrentToken(token,line_no,line_no,start_index,start_index+1);
                        // advance();
                        // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_OPERATOR_COLOR,line_no,line_no,start_index,start_index+1}));

                        continue;
                    }
                    std::string a = std::string(1,current_char);
                    Python_Token* error_token = new Python_Token{a,a,PYTHON_TOKEN_TYPES::PYTHON_ERROR,line_no,line_no,start_index,start_index+1};
                    // style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_ERROR_COLOR,line_no,line_no,start_index-1,start_index}));
                    style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{PYTHON_ERROR_COLOR,line_no,line_no,start_index,start_index+1}));

                    python_tokens.push_back(error_token);
                    getCurrentToken(error_token,line_no,line_no,start_index,start_index+1);
                    advance();
                }

            }
            return python_tokens;
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