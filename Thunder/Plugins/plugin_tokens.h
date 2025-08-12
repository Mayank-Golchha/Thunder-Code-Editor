#ifndef PLUGIN_TOKENS_H
#define PLUGIN_TOKENS_H

#include <windows.h>
#include <string>

struct AutoCompleteToken{
    std::string key;
    std::string value;
};

struct TokenStyle{
    COLORREF color;
    int startLine;
    int endLine;
    int start;
    int end;
};

//will store details like x_offset when u press enter
//for some programming languages like python after : need to add a tab
struct EditorPluginCodeDetails{
    int enter_x_offset;
    std::string enter_tab_char; //the string after which enter_x_offset need to be activated
    //have something to store key and value for ( and ) and " and " and more
};

#endif