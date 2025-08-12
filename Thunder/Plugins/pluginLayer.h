//TODO : free memory of ast

#include "Text_Plugin/text_plugin.h"
#include "Json_Plugin/json_plugin.h"
#include "Python_Plugin/python_plugin.h"
#include "C_Plugin/c_plugin.h"
#include "../language_type.h"
#include "../editor_resources.h"

class PluginManger{
    public:
        Plugin plugin;
        JsonPlugin json_plugin;
        PythonPlugin python_plugin;
        CPlugin c_plugin;

        // std::string language; //programming language -> reguired to know which plugin to activate
        LANGUAGE_TYPE language; //programming language -> reguired to know which plugin to activate
        const std::vector<std::unique_ptr<AutoCompleteToken>>* auto_complete_tokens = nullptr;
        const std::vector<std::shared_ptr<TokenStyle>>* style_tokens = nullptr;
        const std::vector<std::shared_ptr<TokenStyle>>* display_tokens = nullptr;
        std::wstring comment = L"";
        std::string current_typed_word = "";

        // void init(std::string language_){
        void init(LANGUAGE_TYPE language_){
            language = language_;
        }

        void load(std::string code,int cursor_x,int cursor_y){
            // if (language == "text"){
            if (language == LANGUAGE_TYPE::TEXT){
                // Plugin plugin;
                logger.info_log("plugin loading");

                plugin.load(code,cursor_x,cursor_y);
                auto_complete_tokens = &plugin.tokens;
                style_tokens = &plugin.getStyleTokens();
                comment = plugin.comment;                
                current_typed_word = plugin.current_typed_word;
                // logger.log("loaded",LOG_TYPE::LOGGER_INFO);
                logger.info_log("plugin loaded");

                // plugin.display(sx,sy,ex,ey);
                // display_tokens = &plugin.getDisplayTokens();
            }
            else if (language == LANGUAGE_TYPE::JSON){
                // JsonPlugin plugin;
                logger.info_log("plugin loading");

                json_plugin.load(code,cursor_x,cursor_y);
                auto_complete_tokens = &json_plugin.getAutoCompleteTokens();
                style_tokens = &json_plugin.getStyleTokens();
                comment = json_plugin.comment;                
                current_typed_word = json_plugin.current_typed_word;
                logger.info_log("plugin loaded");
                // json_plugin.display(sx,sy,ex,ey);
                // display_tokens = &json_plugin.getDisplayTokens();
            }
            else if (language == LANGUAGE_TYPE::PYTHON){
                // python plugin
                logger.info_log("plugin loading");

                python_plugin.load(code,cursor_x,cursor_y);
                // python_plugin.load(code);
                auto_complete_tokens = &python_plugin.getAutoCompleteTokens();
                style_tokens = &python_plugin.getStyleTokens();
                comment = python_plugin.comment;   
                current_typed_word = python_plugin.current_typed_word;
                logger.info_log("plugin loaded");
                // python_plugin.display(sx,sy,ex,ey);
                // display_tokens = &python_plugin.getDisplayTokens();
            }
            else if (language == LANGUAGE_TYPE::C){
                // c plugin 
                logger.info_log("plugin loading");

                c_plugin.load(code,cursor_x,cursor_y);
                // python_plugin.load(code);
                auto_complete_tokens = &c_plugin.getAutoCompleteTokens();
                style_tokens = &c_plugin.getStyleTokens();
                comment = c_plugin.comment;                
                current_typed_word = c_plugin.current_typed_word;
                logger.info_log("plugin loaded");
                // python_plugin.display(sx,sy,ex,ey);
                // display_tokens = &python_plugin.getDisplayTokens();
            }
        }
        
        void display(int sx,int sy,int ex,int ey){
            if (language == LANGUAGE_TYPE::TEXT){
                plugin.display(sx,sy,ex,ey);
                display_tokens = &plugin.getDisplayTokens();
            }
            else if (language == LANGUAGE_TYPE::JSON){
                json_plugin.display(sx,sy,ex,ey);
                display_tokens = &json_plugin.getDisplayTokens();
            }
            else if (language == LANGUAGE_TYPE::PYTHON){
                python_plugin.display(sx,sy,ex,ey);
                display_tokens = &python_plugin.getDisplayTokens();
            }
            else if (language == LANGUAGE_TYPE::C){
                c_plugin.display(sx,sy,ex,ey);
                display_tokens = &c_plugin.getDisplayTokens();
            }
            // python_plugin.display(sx,sy,ex,ey);

        }

        int isLineCommented(std::wstring& line){
            int index = 0;
            while (line[index] == L' ' && index < line.size()) index++;
            if (index + comment.size() < line.size()){
                if (line.find(comment) == index) return 1;
            }
            return 0;
        }

        int comment_line(std::wstring& line,int uncomment = 1){
            //uncomment means can uncomment also
            //will comment the given line
            // return;
            if (comment != L""){
                int index = 0;
                while (line[index] == L' ' && index < line.size()) index++;
                if (index + comment.size() < line.size()){
                    if (uncomment && line.find(comment) == index){
                        //it is alread a comment remove the comment
                        if (index + 1 + comment.size() < line.size() && line[index + comment.size()] == L' ') line.erase(index,comment.size()+1);
                        else line.erase(index,comment.size()); 
                        return 1;
                    }
                }
                line.insert(index,comment + L" ");
                return 1;
            }
            return 0;
        }
};