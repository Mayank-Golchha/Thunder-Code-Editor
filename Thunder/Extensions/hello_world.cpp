// g++ -std=c++17 -shared -o hello_world.dll hello_world.cpp -lgdi32

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <functional>
#include <iostream>

#include "../api/EditorAPI.h"


EditorAPI* api = nullptr;


void onCharTyped(wchar_t ch){
    if (ch == L'h'){
        POINT p;
        api->getCaretPosition(&p);
        api->insertText(L"hello world",p.x,p.y);
        api->updateEditorContent();
    }
}


std::string wstringstring(std::wstring content){
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &str[0], size_needed, nullptr, nullptr);
    str.pop_back();
    return str;
}

class TodoExtension{
    public:
        void getAllTodos(wchar_t ch){
            // will get all todos
            for (int i = 0;i<api->getEditorContent().size();i++){
                std::wstring line = api->getEditorContent()[i];

                if (line.find(L"TODO") != std::wstring::npos){
                    std::cout << wstringstring(line) << " \n"; 
                }
            }
        }
        
};


void getAllTodos(wchar_t ch){
    std::cout << "called";
    // will get all todos
    for (int i = 0;i<api->getEditorContent().size();i++){
        std::wstring line = api->getEditorContent()[i];

        if (line.find(L"TODO") != std::wstring::npos){
            std::cout << wstringstring(line) << " \n"; 
        }
    }
}


extern "C" __declspec(dllexport)
void RegisterExtension(EditorAPI* api_) {
    api = api_;
    TodoExtension ext;
    api->registerCharTypedCallback(onCharTyped); // this needs to be called if need to run a function that runs when the user writes something in the editor
 
    // api->registerCharTypedCallback([&](wchar_t ch) {
    // ext->getAllTodos(ch);}); // this needs to be called if need to run a function that runs when the user writes something in the editor
 
    // api->registerCharTypedCallback(getAllTodos(api->getEditorContent())); // this needs to be called if need to run a function that runs when the user writes something in the editor
    // api->registerCharTypedCallback(getAllTodos); // this needs to be called if need to run a function that runs when the user writes something in the editor
    api->registerCharTypedCallback(std::bind(&TodoExtension::getAllTodos,&ext,std::placeholders::_1)); // this needs to be called if need to run a function that runs when the user writes something in the editor

    // extern void RegisterCharTypedCallback(void (*cb)(wchar_t));
    // RegisterCharTypedCallback(onCharTyped);
}