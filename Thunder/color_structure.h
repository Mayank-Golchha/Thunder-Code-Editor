#ifndef COLOR_STRUCTURE_H
#define COLOR_STRUCTURE_H

#include <windows.h>

struct Theme{
    COLORREF mainWindow;
    COLORREF titlebar;
    //close maximize and minimize button
    COLORREF windowControlButtonsbk; //background color
    COLORREF windowControlButtonstc; //text color 
    COLORREF hoverButtonColor; //hover color for button 
    COLORREF hoverWindowColorbk; //background color of hover window. The window that will come when you hover over something
    COLORREF hoverWindowColortc; //text color of hover window. The window that will come when you hover over something
    COLORREF menucolorbk; //menu and submenu background color
    COLORREF menucolortc; //menu and sub menu text color
    COLORREF menucolorhoverbk; // menu hover bk color
    COLORREF menucolorhovertc; // menu hover tc color
    COLORREF hoverwindowhighlightline; //menu and sub menu text color   --> the line that will come at top of window when selected
    COLORREF filetabContainer; //file tab container
    COLORREF filetabbk; //file tab background color
    COLORREF filetabtc; //file tab text color
    COLORREF filetabhoverbk; //file tab hover bk color
    COLORREF filetabhovertc; //file tab hover tc color
    COLORREF scrollbarbk; //scroll bar bk
    COLORREF scrollbarthumb; //scroll bar thumb
    COLORREF editorbk; //editor bk color
    COLORREF editortc; //editor tc color
    COLORREF editorSelectedLinebk; //current selected line bk color
    COLORREF editorSelectedTextbk; //selected text bk color in editor
    COLORREF autoCompleteOptionbk; //autocomplete option bk color
    COLORREF autoCompleteOptiontc; //autocomplete option tc color
    COLORREF autoCompleteOptionHoverbk; //autocomplete hover bk color
    COLORREF autoCompleteOptionHovertc; //autocomplete hover tc color
    COLORREF miniCodeViewerbk; //mini code viewer bk color
    COLORREF miniCodeViewertc; //mini code viewer tc color
    COLORREF fileExplorerBk; //file explorer bk color
    COLORREF fileExplorerItemBk; //file explorer items bk color -> same as prev for good looks
    COLORREF fileExplorerItemTc; //file explorer items tc color
    COLORREF fileExplorerHoverItemBk; //file explorer hover item bk color
    COLORREF fileExplorerHoverItemTc; //file explorer hover items tc color
    COLORREF fileExplorerItemSelectedBk; //file explorer item selected bk color
    COLORREF fileExplorerItemSelectedTc; //file explorer item selected tc color
    COLORREF inputWindowBk; //input window bk color
    COLORREF inputWindowTc; //input window tc color
    COLORREF contextMenuBk; //context menu bk color
    COLORREF contextMenuTc; //context menu tc color
    COLORREF contextMenuHighLightBk; //context menu hover bk color
    COLORREF contextMenuHighLightTc; //context menu hover tc color
};


#endif