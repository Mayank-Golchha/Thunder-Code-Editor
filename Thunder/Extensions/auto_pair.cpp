// this extension will auto pair " " , { } , [ ] , ( )
// when the first character is typed second character will be automatically typed

// g++ -std=c++17 -shared -o hello_world.dll hello_world.cpp -lgdi32

#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>

#include "../api/EditorAPI.h"


EditorAPI* api = nullptr;


class AutoPairExtension{
    public:
        void autoPair(wchar_t ch){
            if (api->getCurrentFileExtension() != "txt"){
                wchar_t end_char = L'\0';
                switch (ch){
                    case L'\'':
                        end_char = L'\'';
                        break;
                    case L'"':
                        end_char = L'"';
                        break;
                    case L'[':
                        end_char = L']';
                        break;
                    case L'{':
                        end_char = L'}';
                        break;
                    case L'(':
                        end_char = L')';
                        break;
                }
                if (end_char){
                    POINT p;
                    api->getCaretPosition(&p);
                    api->insertText(std::wstring(1,end_char),p.x,p.y);
                    api->updateEditorContent();
                }
            }
        }
        
};


extern "C" __declspec(dllexport)
void RegisterExtension(EditorAPI* api_) {
    api = api_;
    AutoPairExtension ext;

    // this needs to be called if need to run a function that runs when the user writes something in the editor
    api->registerCharTypedCallback(std::bind(&AutoPairExtension::autoPair,&ext,std::placeholders::_1));

}