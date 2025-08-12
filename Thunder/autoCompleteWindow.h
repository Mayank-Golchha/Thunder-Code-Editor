//TODO : handle up arrow also

#ifndef AUTOCOMPLETEWINDOW_H
#define AUTOCOMPLETEWINDOW_H

#include <vector>
#include <string>

#include "colors.h"
#include "window_id.h"


struct autoCompleteData{
    // std::vector<std::unique_ptr<AutoCompleteToken>>*options;
    const std::vector<std::unique_ptr<AutoCompleteToken>>*options;
    int currentOption;
    int topOption;
    std::string searched_data;
};

HFONT autoCompletWindowFont = CreateFont(
    -AUTO_COMPLETE_OPTION_FONT_HEIGHT, 0, 0, 0,           // Height, Width (0 = auto)
    // FW_NORMAL,              // Weight
    AUTO_COMPLETE_OPTION_FONT_WEIGHT,
    FALSE, FALSE, FALSE,    // Italic, Underline, StrikeOut
    ANSI_CHARSET,           // Character set
    OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY,
    FIXED_PITCH | FF_MODERN,// Fixed-pitch monospaced
    L"Consolas"             // Font name
);

LRESULT CALLBACK autoCompleteWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    autoCompleteData* data = (autoCompleteData*)GetProp(hWnd, L"autoCompleteProp");
    
    switch (msg){
        case WM_DESTROY:
        {
            RemoveProp(hWnd, L"autoCompleteProp");
            return 0;
        }

        case AUTO_COMPLETE_DOWN_ARROW:
        {
            //this message is sent by editor to move down
            if (data->currentOption + 1 < data->options->size()) {
                data->currentOption += 1;
                // if (data->currentOption == AUTO_COMPLETE_MAX_OPTIONS-1 && data->topOption+AUTO_COMPLETE_MAX_OPTIONS < data->options->size()){
                if (data->currentOption > AUTO_COMPLETE_MAX_OPTIONS-1 && data->topOption+AUTO_COMPLETE_MAX_OPTIONS < data->options->size()){
                    data->topOption += 1;
                }                
            }
            else {
                data->currentOption = 0;
                data->topOption = 0;
            }
            InvalidateRect(hWnd,NULL,TRUE);
            
            return 0;
        }

        case AUTO_COMPLETE_UP_ARROW:
        {
            if (data->currentOption - 1 >= 0){
                data->currentOption -= 1;
                if (data->currentOption+1 == data->topOption){
                    data->topOption -= 1;
                }
            }
            else{
                data->currentOption = 0;
                data->topOption = 0;
            }
            InvalidateRect(hWnd,NULL,TRUE);

            return 0;
        }

        case AUTO_COMPLETE_ENTER:
        {
            if (data->options->size() && data->currentOption < data->options->size()){
                // editContent[cursorY].erase(cursorX-data->searched_data.size(),cursorX);
                std::wstring wstr = string_to_wstring((*data->options)[data->currentOption]->value);
                // editContent[cursorY].insert(cursorX, wstr);
                editContent[cursorY].replace(cursorX-data->searched_data.size(),data->searched_data.size(),wstr);
                // cursorX += wstr.size();
                cursorX += wstr.size() - data->searched_data.size();
                InvalidateRect(hEditor,NULL,TRUE);
                SetCaretPosition2(cursorX,cursorY);
                DestroyWindow(hWnd);
            }
            return 0;
        }

        case WM_PAINT:
        {
            if (!data->options->size()){
                DestroyWindow(hWnd);
                return 0;
            }
            PAINTSTRUCT ps;
            HBRUSH brush,optionBrush,hoverBrush;
        
            brush = CreateSolidBrush(RGB(100,80,0));
            optionBrush = CreateSolidBrush(colorTheme.autoCompleteOptionbk);
            hoverBrush = CreateSolidBrush(colorTheme.autoCompleteOptionHoverbk);

            HDC hdc = BeginPaint(hWnd, &ps);

            RECT rc;
            GetClientRect(hWnd,&rc);

            FillRect(hdc, &rc, brush);

            SetBkMode(hdc, TRANSPARENT);

            SelectObject(hdc,autoCompletWindowFont);

            SetTextColor(hdc, colorTheme.autoCompleteOptiontc);


            // for (int i = data->topOption; data->options->size() && i<AUTO_COMPLETE_MAX_OPTIONS && i < data->options->size();i++){
            for (int i = data->topOption; data->options->size() && i < data->options->size();i++){
                rc.top = (i-data->topOption)*AUTO_COMPLETE_OPTION_HEIGHT;
                rc.bottom = rc.top + AUTO_COMPLETE_OPTION_HEIGHT;
                
                if (i == data->currentOption) FillRect(hdc, &rc, hoverBrush);
                else FillRect(hdc, &rc, optionBrush);
                
                // TextOut(hdc, 5, rc.top+1,(*(data->options))[i].c_str(), wcslen((*(data->options))[i].c_str()));
                // const wchar_t* wchar = string_to_wstring(data->options[i]->key).c_str();
                const wchar_t* wchar = string_to_wstring((*data->options)[i]->key).c_str();
                TextOut(hdc, 5, rc.top+1,wchar, wcslen(wchar));
            }


            DeleteObject(brush);
            EndPaint(hWnd, &ps);
            return 0;
        }
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}

HWND createAutoCompleteWindow(HWND hWndparent,int x,int y,const std::vector<std::unique_ptr<AutoCompleteToken>>& autoCompleteTokens,std::string current_typed_word){
    if (hautoComplete){
        DestroyWindow(hautoComplete);
        hautoComplete = NULL;
    }
    if (!autoCompleteTokens.size() || autoCompleteTokens[0]->key == "") {
        return NULL;
    }
    SIZE size;
    HDC hdc = GetDC(hWndparent);
    std::wstring text;
    for (int i = 0;i<autoCompleteTokens.size();i++) text = (text.size() > autoCompleteTokens[i]->key.size())?text:string_to_wstring(autoCompleteTokens[i]->key);
    // for (int i = 0;i<autoCompleteTokens->size();i++) text = (text.size() > (*autoCompleteTokens)[i]->key.size())?text:string_to_wstring((*autoCompleteTokens)[i]->key);
    GetTextExtentPoint32(hdc, text.c_str(), wcslen(text.c_str()), &size); //width of font

    // int h = (autoCompleteTokens.size()>AUTO_COMPLETE_MAX_OPTIONS)?AUTO_COMPLETE_MAX_OPTIONS:options.size();
    int h = (autoCompleteTokens.size()>AUTO_COMPLETE_MAX_OPTIONS)?AUTO_COMPLETE_MAX_OPTIONS:autoCompleteTokens.size();

    //cx + 50  -> 50 is extra space
    HWND w = CreateWindowEx(0 ,L"AutoCompleteWindow",L"",WS_VISIBLE | WS_CHILD | WS_BORDER,x,y,size.cx+50,h*AUTO_COMPLETE_OPTION_HEIGHT,hWndparent,NULL,NULL,NULL);
    // autoCompleteData* data = new autoCompleteData{&autoCompleteTokens,0,0};
    autoCompleteData* data = new autoCompleteData{&autoCompleteTokens,0,0,current_typed_word};
    SetProp(w, L"autoCompleteProp", (HANDLE)data);
    return w;
}


#endif