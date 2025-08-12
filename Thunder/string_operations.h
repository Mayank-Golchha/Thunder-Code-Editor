#ifndef STRING_OPERATIONS_H
#define STRING_OPERATIONS_H

#include <windows.h>
#include <string>
#include <wchar.h>
#include <iostream>
#include <vector>

const wchar_t* string_to_wchar(std::string word){
    std::wstring widestr = std::wstring(word.begin(),word.end());
    const wchar_t* widecstr = widestr.c_str();
    return widecstr;
}

std::wstring string_to_wstring(std::string word){
    std::wstring widestr = std::wstring(word.begin(),word.end());
    return widestr;
}

std::wstring combineLineContent(std::vector<std::wstring> code,int sy,int ey){
    //will combine all lines to one line with \n
    size_t total_size = 0;
    if (code.size() == 1 && !code[0].size()) return L"";
    for (int i = sy;i<ey;i++) total_size += code[i].size()+1;
    
    std::wstring content;
    content.reserve(total_size);

    for (int i = sy;i<ey;i++) {
        content += code[i];
        content += L"\n";
    }

    return content;
}

std::string wstring_string(std::wstring content){
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &str[0], size_needed, nullptr, nullptr);
    str.pop_back();
    return str;
}

void current_tokens_word(std::vector<std::wstring>&content,int cursor_x,int cursor_y,int *start,int* end){
    //when you double click the word you gets selected this function is responsible to return the coordinates of the word
    int i = cursor_x,j = cursor_x;
    while (i >= 0){
        i--;
        if ((content[cursor_y][i] >= L'a' && content[cursor_y][i] <= L'z') || (content[cursor_y][i] >= L'A' && content[cursor_y][i] <= L'Z') || (content[cursor_y][i] >= L'0' && content[cursor_y][i] <= L'9') || content[cursor_y][i] == L'_') continue;
        else break;
    }
    while (j < content[cursor_y].size()){
        j++;
        if ((content[cursor_y][j] >= L'a' && content[cursor_y][j] <= L'z') || (content[cursor_y][j] >= L'A' && content[cursor_y][j] <= L'Z') || (content[cursor_y][j] >= L'0' && content[cursor_y][j] <= L'9') || content[cursor_y][j] == L'_') continue;
        else break;
    }
    *start = i+1;
    *end = j;
}

int countChar(std::wstring text,int s,int e,wchar_t t){
    //will count from s if encounter another character will break
    int c = 0;
    for (int i = s;i<e;i++) {
        if (text[i] == t) c++;
        else break;
    }
    return c;
}

void removeAtIndex(std::wstring& text,int index){
    if (index >= 0 && index < text.size()) {
        text.erase(index, 1);
    }
    // text[len-1] = L'\0'; // optional, clean end

}


#endif