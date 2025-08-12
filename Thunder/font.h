//TODO : A bug will appear at places where calculateFontWidth function is used because it calculating width for main window text

#ifndef FONT_H
#define FONT_H

#include <windows.h>

HFONT customFont(int fontSize,int width,int weight,const wchar_t* fontName = L"Segoe UI"){
    // weights 0 - 1000
    // FW_DONTCARE 0,FW_THIN 100,FW_EXTRALIGHT 200,FW_ULTRALIGHT 200,FW_LIGHT 300,FW_NORMAL 400,FW_REGULAR 400,FW_MEDIUM 500,FW_SEMIBOLD 600,FW_DEMIBOLD 600
    // FW_BOLD 700,FW_EXTRABOLD 800,FW_ULTRABOLD 800,FW_HEAVY 900,FW_BLACK 900
        
    //fontName -> L"Segoe UI"
    //width -> average width of each character
    
    HFONT f = CreateFontW(
        -fontSize, width, 0, 0, weight, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        // DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        fontName
    );
    return f;
}

int calculateFontWidth(const wchar_t* text,HFONT font) {
    HDC hdc = GetDC(hmainWindow); // Or any valid window
    SelectObject(hdc, font);

    SIZE size;
    GetTextExtentPoint32(hdc, text, wcslen(text), &size);

    int width = size.cx + FILE_TAB_CLOSE_BUTTON_WIDTH + 16; // 16px padding

    ReleaseDC(hmainWindow, hdc);
    // DeleteObject(font);

    return width;
}


int fontWidth(HDC hdc,const wchar_t* text,HFONT font) {
    SIZE size;
    GetTextExtentPoint32(hdc, text, wcslen(text), &size);
    return size.cx;
}

#endif
