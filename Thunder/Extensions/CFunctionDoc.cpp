// Displays quick information about built-in C functions.
// When you type or hover over a standard C function (like printf, malloc, strcpy, etc.), this extension shows its usage, parameters, and a short description.
// 💡 Note: Only works with standard library functions (e.g., stdio.h, stdlib.h, string.h, etc.).


//  g++ -std=c++17 -shared -o CFunctionDoc.dll CFunctionDoc.cpp -lgdi32 -lmsimg32        

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <functional>
#include <unordered_map>

#include "../api/EditorAPI.h"


static EditorAPI* api = nullptr;

HWND hoverWindow = NULL;


void onCharTyped(wchar_t ch){
    if (ch == L'h'){
        POINT p;
        api->getCaretPosition(&p);
        api->insertText(L"hello world",p.x,p.y);
        api->updateEditorContent();
    }
}

class DocGenerator{
    public:
        TextDisplayWindow hoverWindow = TextDisplayWindow{};
        
        std::unordered_map<std::string, std::wstring> cStringFunctionDocs = {
            {"memchr", L"Locate first occurrence of a byte in a memory block."},
            {"memcmp", L"Compare two memory blocks byte-by-byte."},
            {"memcpy", L"Copy a block of memory (nonoverlapping safe)."},
            {"memmove", L"Copy a block of memory; safe if source and dest overlap."},
            {"memset", L"Fill a block of memory with a specified byte value."},
            {"strcat", L"Append the source C string to the destination string."},
            {"strchr", L"Find first occurrence of a character in a C string."},
            {"strcmp", L"Compare two C strings lexicographically."},
            {"strcoll", L"Compare strings using the current locale collation rules."},
            {"strcpy", L"Copy C string, including null terminator, to destination."},
            {"strcspn", L"Return span of initial characters in str1 not containing any in str2."},
            {"strerror", L"Return pointer to error-message string for errno value."},
            {"strlen", L"Return length of C string, excluding null terminator."},
            {"strncat", L"Append up to n characters from source to destination C string."},
            {"strncmp", L"Compare up to n characters of two C strings."},
            {"strncpy", L"Copy up to n characters of source to destination C string."},
            {"strpbrk", L"Locate first occurrence in str1 of any character in str2."},
            {"strrchr", L"Find last occurrence of a character in a C string."},
            {"strspn", L"Return length of initial segment in str1 consisting only of characters in str2."},
            {"strstr", L"Find first occurrence of substring needle in haystack."},
            {"strtok", L"Split string into tokens based on delimiter set."},
            {"strxfrm", L"Transform a string according to locale for collation comparisons."}
        };
        
        
        void Hello(POINT *p){
            std::string e = api->getCurrentFileExtension();
            if (e == "c" || e == "h"){
                api->MouseToCursor(p);
                POINT screenPos = *p;
                api->CursorToAppPosition(&screenPos);
                std::string word = api->wstringToString(getCurrentWord(*p));
                auto it = cStringFunctionDocs.find(word);
                if (it != cStringFunctionDocs.end()){
                        if (hoverWindow.x == screenPos.x && hoverWindow.y == screenPos.y) return;
                        if (hoverWindow.getHandle()) hoverWindow.destroy();
                        hoverWindow.create(hEditor,screenPos.x,screenPos.y,200,50,RGB(0,0,0),RGB(200,200,200));
                        hoverWindow.setText(it->second);
                        hoverWindow.show();
                }
                else hoverWindow.destroy();
            }
        }

        std::wstring getCurrentWord(POINT p){
            // will get the the current word
            std::vector<std::wstring> content = api->getEditorContent();
            if (p.y < content.size() && p.x < content[p.y].size()){
                int i = p.x-1,j = p.x+1;
                while (i >= 0 && ((content[p.y][i] >= L'a' && content[p.y][i] <= L'z') || (content[p.y][i] >= L'A' && content[p.y][i] <= L'Z') || (content[p.y][i] >= L'0' && content[p.y][i] <= L'9') || content[p.y][i] == L'_')) i--;
                while (j < content[p.y].size() && ((content[p.y][j] >= L'a' && content[p.y][j] <= L'z') || (content[p.y][j] >= L'A' && content[p.y][j] <= L'Z') || (content[p.y][j] >= L'0' && content[p.y][j] <= L'9') || content[p.y][j] == L'_')) j++;
                
                return content[p.y].substr(i+1,j-i-1);
            }
            return L"";
        }
};


extern "C" __declspec(dllexport)
void RegisterExtension(EditorAPI* api_) {
    api = api_;
    static DocGenerator ext;

    api->registerOnMouseMoveCallback([&](POINT* p) {
        ext.Hello(p);
    }); 

}