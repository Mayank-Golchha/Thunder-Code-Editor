#ifndef COLORHANDLING_H
#define COLORHANDLING_H

#include <windows.h>
#include <string>
#include <vector>

#include "string_operations.h"

struct ColorStructure{
    COLORREF txtcolor;
    COLORREF bkcolor;
    std::string word;
    int x;
    int y;
};

// const wchar_t* string_to_wchar(std::string word){
//     std::wstring widestr = std::wstring(word.begin(),word.end());
//     const wchar_t* widecstr = widestr.c_str();
//     return widecstr;
// }

void HandleColor(HDC hdc,std::vector<ColorStructure*> cs){
    for (ColorStructure* w : cs){
        SetTextColor(hdc, w->txtcolor);
        SetBkColor(hdc,w->bkcolor);
        TextOut(hdc, w->x, w->y, string_to_wchar(w->word), w->word.length());
    }
}

#endif