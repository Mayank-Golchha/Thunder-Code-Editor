#ifndef PLUGINS_HELPER_FUNCTION_H
#define PLUGINS_HELPER_FUNCTION_H

#include <string>

class PluginHelper{
    public:
        int match(std::string text,std::string word,int index){
            //will match whether word in text after index
            if (index + word.size() >= text.size()) return 0;
            for (int i = index;i<index + word.size();i++){
                if (text[i] != word[i-index]) return 0;
            }
            return 1;
        }

        int word_matcher(std::string s1,std::string s2){
            //s1 is the current string that needs to be matched
            //this function will return the percent s1 match s2
            //if size of s2 is smaller than s1 than there will be no match
            //i.e for match size of s2 >= size of s1
            int m = 0;
            int l1 = s1.size(),l2 = s2.size();
            if (l2 < l1) return 0;
            for (int i = 0;i < l2 && m < l1;i++) if (s2[i] == s1[m]) m++;
            return 100*m/l2;
        }

        int string_token(std::string text,int index){
            //the word that starts with " and ends with "
            //will return the words length
            int s = index;
            while (index < text.size() && text[index] != '"'){
                if (text[index] == '\n'){
                    index--;
                    break;
                }
                index++;
            }
            index++;
            return index-s;
        }

        int number_token(std::string text,int index){
            //will check if the word from index is a no
            //a no can be a integer and float
            //doesnot support exponent
            //will return length of word
            int s = index;
            while (index < text.size() && text[index] >= '0' && text[index] <= '9') index++;
         
            return index-s;
        }

};


#endif