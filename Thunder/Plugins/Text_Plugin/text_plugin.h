//we will need to parse all tokens to autocomplete tokens

#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include "colors.h"
#include "E:/Text Editor/Plugins/plugin_tokens.h" //bug here dont put actual path
// #include "../Plugins/plugin_tokens.h" //bug here dont put actual path


class Plugin{
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

        const  std::vector<std::shared_ptr<TokenStyle>>& getStyleTokens() const {
            return style_tokens;
        }

        const  std::vector<std::shared_ptr<TokenStyle>>& getDisplayTokens() const {
            return display_tokens;
        }

        void removeSameAutoCompleteTokens(std::vector<std::unique_ptr<AutoCompleteToken>>& autoTokens){
            //will remove similar tokens
            for (int i = autoTokens.size()-1;i>0;i--){
                if (autoTokens[i]->key == autoTokens[i-1]->key) autoTokens.erase(autoTokens.begin()+i);
            } 
        }

        void quickSort(std::vector<std::unique_ptr<AutoCompleteToken>>& autoTokens,int arr[],int start,int end){
            if (start >= end) return;
            int i = start-1,j = start;
            int pivot = arr[end];
            while (j < end){
                if (arr[j] > pivot) {
                    i++;
                    int temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                    std::swap(autoTokens[i],autoTokens[j]);
                }
                j++;
            }
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            std::swap(autoTokens[i],autoTokens[j]);
            quickSort(autoTokens,arr,start,i-1);
            quickSort(autoTokens,arr,i+1,end);
        }

        int match(std::string s1,std::string s2){
            //s1 is the current string that needs to be matched
            //this function will return the percent s1 match s2
            //if size of s2 is smaller than s1 than there will be no match
            //i.e for match size of s2 > size of s1
            int m = 0;
            int l1 = s1.size(),l2 = s2.size();
            if (l2 < l1) return 0;
            for (int i = 0;i < l2 && m < l1;i++) if (s2[i] == s1[m]) m++;
            return 100*m/l2;
        }

        void auto_complete_list(std::vector<std::unique_ptr<AutoCompleteToken>>& list,std::vector<std::string>*words){
            //will return list of strings that match the criteria
            //will return in the correct order of priority
            int arr[words->size()], j = 0;
            for (int i = 0;i<words->size();i++){
                int m = match(current_typed_word,(*words)[i]);
                if (m > 0){
                    // std::unique_ptr<AutoCompleteToken> token = std::make_unique<AutoCompleteToken>((*words)[i],(*words)[i]);
                    // list.push_back(std::move(token));
                    
                    tokens.push_back(std::make_unique<AutoCompleteToken>(AutoCompleteToken{(*words)[i], (*words)[i]}));

                    arr[j++] = m;
                }
            }
            if (list.size()) {
                quickSort(list,arr,0,list.size()-1);
                //will remove similar tokens
                removeSameAutoCompleteTokens(list);
            }
        }

        void load(std::string code,int cursor_x,int cursor_y){
            //will split code
            current_typed_word = "";
            current_string = "";
            clear();
            len = code.size();
            std::vector<std::string> words;
            
            int line_no = 0,start_index = 0,last = 0;
            for (int i = 0;i<len;i++){
                if (code[i] == ' ' || code[i] == '\n'){
                    if (i > last){//if \n dont put in words
                        std::string word = code.substr(last,i-last);
                        int s = start_index - (i-last);
                        int e = start_index;
                        words.push_back(word);

                        style_tokens.push_back(std::make_shared<TokenStyle>(TokenStyle{TEXT_COLOR,line_no,line_no,s,e+1}));

                        // if (cursor_y == line_no && cursor_x >= s && cursor_x <= e){
                        if (cursor_y == line_no && cursor_x > s && cursor_x <= e){
                            current_string = word;
                            if (cursor_x == len) current_typed_word = current_string;
                            else current_typed_word = word.substr(0,cursor_x-s+1);
                        }
                    }
                    last = i+1;

                }
                start_index++;
                if (code[i] == '\n'){
                    line_no++;
                    start_index = 0;
                }
            }   
            auto_complete_list(tokens,&words);
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
            if (style_tokens[i]->startLine >= sy && style_tokens[i]->endLine <=ey && style_tokens[i]->end >= sx && style_tokens[i]->start <= ex) display_tokens.push_back(style_tokens[i]);
        }
    }

};