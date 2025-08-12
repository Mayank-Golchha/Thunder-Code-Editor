// this extension will auto complete a html tag
// when the starting tag is completed it inserts the end tag

// g++ -std=c++17 -shared -o html_tag_completer.dll html_tag_completer.cpp -lgdi32

#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>

#include "../api/EditorAPI.h"


EditorAPI* api = nullptr;


class Html_Tag_Completer{
    public:
        std::wstring tag = L"";
        int s = 0;

        void char_typed(wchar_t c){
            if (api->getCurrentFileExtension() == "html"){
                if (c == L'<') {
                    s = 1;
                    tag = L"";
                }
                else if (c == L'>'){ 
                    s = 0;
                    std::wstring end_tag = L"</" + tag + L">";
                    POINT p;
                    api->getCaretPosition(&p);
                    api->insertText(end_tag,p.x,p.y);
                    tag = L"";
                }
                else if (c) tag += c;
            }
        }

};



extern "C" __declspec(dllexport)
void RegisterExtension(EditorAPI* api_) {
    static Html_Tag_Completer ext;
    api = api_;
    api->registerCharTypedCallback([&](wchar_t ch) {
        ext.char_typed(ch);});

    // AutoPairExtension ext;
    // api->registerCharTypedCallback(std::bind(&TodoExtension::getAllTodos,&ext,std::placeholders::_1)); // this needs to be called if need to run a function that runs when the user writes something in the editor

    // this needs to be called if need to run a function that runs when the user writes something in the editor
    // api->registerCharTypedCallback(std::bind(&AutoPairExtension::autoPair,&ext,std::placeholders::_1));

}