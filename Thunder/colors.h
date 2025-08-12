#ifndef COLORS_H
#define COLORS_H

#include <windows.h>

#include "color_structure.h"
#include "ui_theme.h"

#define BLACK RGB(0,0,0)
#define WHITE RGB(255,255,255)
#define RED RGB(255,0,0)
#define GREEN RGB(0,255,0)
#define BLUE RGB(0,0,255)
#define YELLOW RGB(255,255,0)
#define LIGHTBLUE RGB(0,255,255)
#define MAGENTA RGB(255,0,255)
#define GREY RGB(128,128,128)


// struct Theme{
//     COLORREF mainWindow;
//     COLORREF titlebar;
//     //close maximize and minimize button
//     COLORREF windowControlButtonsbk; //background color
//     COLORREF windowControlButtonstc; //text color 
//     COLORREF hoverButtonColor; //hover color for button 
//     COLORREF hoverWindowColorbk; //background color of hover window. The window that will come when you hover over something
//     COLORREF hoverWindowColortc; //text color of hover window. The window that will come when you hover over something
//     COLORREF menucolorbk; //menu and submenu background color
//     COLORREF menucolortc; //menu and sub menu text color
//     COLORREF hoverwindowhighlightline; //menu and sub menu text color   --> the line that will come at top of window when selected
//     COLORREF filetabContainer; //file tab container
//     COLORREF filetabbk; //file tab background color
//     COLORREF filetabtc; //file tab text color
//     COLORREF filetabhoverbk; //file tab hover bk color
//     COLORREF filetabhovertc; //file tab hover tc color
//     COLORREF scrollbarbk; //scroll bar bk
//     COLORREF scrollbarthumb; //scroll bar thumb
//     COLORREF editorbk; //editor bk color
//     COLORREF editortc; //editor tc color
//     COLORREF editorSelectedLinebk; //current selected line bk color
//     COLORREF editorSelectedTextbk; //selected text bk color in editor
//     COLORREF autoCompleteOptionbk; //autocomplete option bk color
//     COLORREF autoCompleteOptiontc; //autocomplete option tc color
//     COLORREF autoCompleteOptionHoverbk; //autocomplete hover bk color
//     COLORREF autoCompleteOptionHovertc; //autocomplete hover tc color
//     COLORREF miniCodeViewerbk; //mini code viewer bk color
//     COLORREF miniCodeViewertc; //mini code viewer tc color
//     COLORREF fileExplorerBk; //file explorer bk color
//     COLORREF fileExplorerItemBk; //file explorer items bk color -> same as prev for good looks
//     COLORREF fileExplorerItemTc; //file explorer items tc color
//     COLORREF fileExplorerItemSelectedBk; //file explorer item selected bk color
//     COLORREF fileExplorerItemSelectedTc; //file explorer item selected tc color
//     COLORREF inputWindowBk; //input window bk color
//     COLORREF inputWindowTc; //input window tc color
//     COLORREF contextMenuBk; //context menu bk color
//     COLORREF contextMenuTc; //context menu tc color
//     COLORREF contextMenuHighLightBk; //context menu hover bk color
//     COLORREF contextMenuHighLightTc; //context menu hover tc color
// };

Theme colorful_theme = {
    RGB(255, 255, 255), //mainwindow
    RGB(200, 180, 0), //titlebar
    RGB(200, 0, 0), //close button b
    RGB(255,255,255), //close button t
    RGB(40,40,40), //close button h
    RGB(0,0,0), //hover window b
    RGB(255,255,255), //hover window t
    RGB(0,0,0), //menubar b
    RGB(255,255,255), //menubar t
    RGB(100,100,100), //menubar hover b
    RGB(255,255,255), //menubar hover t
    RGB(0,255,0), //hightlight line --> to be shown on selecting a window on top
    RGB(50,50,50), //file tab container
    RGB(0,0,0), //file tab b
    RGB(255,255,255), //file tab t 
    RGB(100,20,50), //file tab hover b
    RGB(40,100,50), //file tab hover t
    RGB(40,200,100), //scroll bar bk
    RGB(255,0,40), //scroll bar thumb
    RGB(50,100,30), //editor b color
    RGB(0,0,0), //editor t color default
    RGB(0,255,100), //current selected line bk color
    RGB(0, 120, 215), //selected text bk color in editor
    RGB(0,0,0), //autocomplete option bk color
    RGB(200,200,200), //autocomplete option tc color
    RGB(0,0,100), //autocomplete option hover bk color
    RGB(255,255,255), //autocomplete option hover tc color
    RGB(0,100,30), //mini code viewer bk color
    RGB(255,255,255), //mini code viewer tc color
    RGB(0,0,0), //file explorer bk color
    RGB(0,0,0), //file explorer item bk color
    RGB(255,255,255), //file explorer item tc color
    RGB(25,25,25), //file explorer hover item tc color
    RGB(5,205,25), //file explorer hover item tc color
    RGB(0,0,0), //file explorer item selected bk color
    RGB(255,255,255), //file explorer item selected tc color
    RGB(0,0,0), //input window bk color
    RGB(200,200,200), //input window tc color
    RGB(0,0,0), //context menu bk color
    RGB(200,200,200), //context menu tc color
    RGB(0,200,200), //context menu hover bk color
    RGB(120,20,200), //context menu hover tc color
};

// Theme colorTheme = colorful_theme;
Theme darkTheme = loadTheme(".thunder/ui/dark_theme.json",colorful_theme);
Theme dark_pink = loadTheme(".thunder/ui/dark_pink_theme.json",colorful_theme);
Theme default_theme = loadTheme(".thunder/ui/ui_colors.json",colorful_theme); //if fails then will load default theme
// colorTheme = loadTheme(".thunder/ui/ui_colors.json",colorful_theme); //if fails then will load default theme
// Theme colorTheme = loadTheme(".thunder/ui/dark_pink_theme.json",colorful_theme); //if fails then will load default theme
// Theme colorTheme = darkTheme;
// colorTheme = dark_pink;
// Theme colorTheme = default_theme;
Theme colorTheme = default_theme;
// Theme colorTheme = dark_pink;

#endif