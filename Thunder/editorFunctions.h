// //this is a helper file of editor.h
// //will only work with fonts with same character width
// //TODO : in GetCursorEditCoordinates function will binary search be more effecient if yes then apply
#ifndef EDITORFUNCTIONS_H
#define EDITORFUNCTIONS_H

#include "dimensions.h"
#include "api/API_Mananger.h"
#include "string_operations.h"
#include "font.h"
#include "colors.h"
#include "window_id.h"
// #include "Plugins/Json_Plugin/syntaxHighlighter.h"
#include "Plugins/pluginLayer.h"
#include "Animation_library/animation.h"
// #include "language_type.h"
#include "Plugins/language_type.h"
#include "editor_resources.h"
#include "file_explorer.h"
// #include "customWindow.h"
#include "hoverWindow.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <mutex>

// float horzThumbX = 0,vertThumbY = 0;
// float horzThumbWidth = 0,vertThumbHeight = 0;

// int editorWidth = 0,editorHeight = 0;

// float scrolledY = 0,scrolledX = 0;

// int hScrolling = 0,vScrolling = 0;

// int cursorX = 0,cursorY = 0;

// int maxLineWidthValue = 0;

// static DWORD last_click_time = 0; //to find the last time when clicked

// //selecting word = 1 while selecting 
// //wordselected = 1 while a word is selected
// int isselectingWord = 0,isWordSelected = 0;

// int startSelectionX = -1,startSelectionY = -1;
// int endSelectionX = -1,endSelectionY = -1;

// std::atomic<int> plugin_task_id = 0;

// HWND hautoComplete = NULL;

// LANGUAGE_TYPE language_type = LANGUAGE_TYPE::TEXT;

// PluginLayer plun;
// Plugin plugin;
// PluginManger plugin;
PluginManger plugin;
std::mutex plugin_mutex;


Animation lightning_animation(L"Animation_library/resources/lightning_2"); //animation on mouse click
// Animation moving_dino(L"Animation_library/resources/dino/dino");
// DinoAnimation moving_dino(L"Animation_library/resources/dino/dino",0,10,1,0);


std::vector<std::wstring> autotext = {L"Hello",L"World",L"I am",L"Mayank",L"Golchha"};


//content of editor
// std::vector<std::wstring> editContent;

struct charInfo{
    wchar_t character;
    COLORREF color;
};

std::vector<std::vector<charInfo>> allcharcolor;

// HFONT editorFont = CreateFont(
//     -EDITOR_FONT_HEIGHT, 0, 0, 0,           // Height, Width (0 = auto)
//     // FW_NORMAL,              // Weight
//     EDITOR_FONT_WEIGHT,
//     FALSE, FALSE, FALSE,    // Italic, Underline, StrikeOut
//     ANSI_CHARSET,           // Character set
//     OUT_DEFAULT_PRECIS,
//     CLIP_DEFAULT_PRECIS,
//     DEFAULT_QUALITY,
//     FIXED_PITCH | FF_MODERN,// Fixed-pitch monospaced
//     L"Consolas"             // Font name
// );


// int editorFontWidth = 0;


int maxLineWidth(){
    //will return the maximum width of line from editContent
    int m = 0;
    for (int i = 0;i<editContent.size();i++) m = (editContent[i].size() > m)?editContent[i].size():m;
    return m;
}

void swap(int* a,int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

std::wstring SUBSTR(const std::wstring& str, size_t pos, size_t len = std::wstring::npos, const char* file = "", int line = 0) {
    if (pos > str.size()) {
        std::wcerr << L"[CRASH] substr out_of_range at " << file << ":" << line
                   << L" — str.size()=" << str.size() << L", pos=" << pos << std::endl;
        return L""; // or throw to halt immediately
    }
    return str.substr(pos, len);
}

void drawCarret(HWND hWnd,int x,int y,int height){
    //default width
    CreateCaret(hWnd, (HBITMAP) NULL, CARRET_WIDTH, height); 

    SetCaretPos(x, y); 

    ShowCaret(hWnd); 
}

void GetColRow(LPPOINT p){
    int leftX = EDITOR_WRITING_PART_X - (int)(scrolledX)%(int)(editorFontWidth);

    int topLine = scrolledY/EDITOR_LINE_HEIGHT;
    int topY = EDITOR_WRITING_PART_Y - (int)(scrolledY)%(int)(EDITOR_LINE_HEIGHT);
    // int drawY = (y-topLine)*EDITOR_LINE_HEIGHT + topY;
    int line = p->y-topLine;
    p->y = line;
    // SetCaretPos(leftX + p->x*editorFontWidth,line*EDITOR_LINE_HEIGHT+topY);
}

// void SetCaretPosition2(int x,int y){
//     //x,y are caret position
//     int leftX = EDITOR_WRITING_PART_X - (int)(scrolledX)%(int)(editorFontWidth);
//     int topY = EDITOR_WRITING_PART_Y - (int)(scrolledY)%(int)(EDITOR_LINE_HEIGHT);
//     int topLine = scrolledY/EDITOR_LINE_HEIGHT;
//     // int l = scrolledX/editorFontWidth;
//     int line = y-topLine;
//     // int lx = x-l;
//     int lx = x-(int)(scrolledX/editorFontWidth);
//     SetCaretPos(leftX + lx*editorFontWidth,line*EDITOR_LINE_HEIGHT+topY);
// }

void SetCaretPosition(int x,int y){
    //x,y are position of mouse not caret
    int topY = EDITOR_WRITING_PART_Y - (int)(scrolledY)%(int)(EDITOR_LINE_HEIGHT);
    int leftX = EDITOR_WRITING_PART_X - (int)(scrolledX)%(int)(editorFontWidth);

    int line = (y-topY)/EDITOR_LINE_HEIGHT;
    int col = (x-leftX)/editorFontWidth;

    int topLine = scrolledY/EDITOR_LINE_HEIGHT;
    int left = scrolledX/editorFontWidth;

    SetCaretPos(leftX + col*editorFontWidth,topY + line*EDITOR_LINE_HEIGHT);

    cursorY = line+topLine;
    cursorX = col+left;

    int c = 0;

    if (cursorY > editContent.size()){
        c = 1;
        cursorY = editContent.size();
    }
    if (cursorX > editContent[cursorY].size()){
        c = 1;
        cursorX = editContent[cursorY].size();
    }
    if (c) SetCaretPosition2(cursorX,cursorY);
}

void Handle_Multiple_Clicks(HWND hWnd){
    //will handle double clicks
    int count = (int)(INT_PTR)GetProp(hWnd,L"DoubleClickCount");
    DWORD current_time = GetMessageTime();

    if (current_time - last_click_time > CLICK_INTERVAL) {
        // Reset if the time gap is too large
        count = 0; 
    }

    last_click_time = current_time;
    if (!count){
        SetTimer(hWnd, 1, GetDoubleClickTime(), NULL);
    }
    count++;
    if (count == 1){
        //will select the current word
        int start = 0,end = 0;
        current_tokens_word(editContent,cursorX,cursorY,&start,&end);
        if (end > start){
            startSelectionX = start;
            endSelectionX = end;
            startSelectionY = cursorY;
            endSelectionY = cursorY;
            isWordSelected = true;
            cursorX = end;
            SetCaretPosition2(cursorX,cursorY);
            //if selected word is whole line then count++ so next time will select whole content
            if (endSelectionX == editContent[cursorY].size()) count++;
        }
    }
    else if (count == 2){
        //will select the current line
        startSelectionX = 0;
        endSelectionX = editContent[cursorY].size();
        startSelectionY = cursorY;
        endSelectionY = cursorY;
        isWordSelected = true;
        cursorX = endSelectionX;
        SetCaretPosition2(cursorX,cursorY); 
    }
    else if (count >= 3){
        //will select whole content
        startSelectionX = 0;
        endSelectionX = editContent[editContent.size()-1].size();
        startSelectionY = 0;
        endSelectionY = editContent.size()-1;
        isWordSelected = true;
        count = 0;
        cursorX = endSelectionX;
        cursorY = endSelectionY;
        SetCaretPosition2(cursorX,cursorY);
        KillTimer(hWnd, 1); // Stop the timer here after triple click

    }
    SetProp(hWnd,L"DoubleClickCount",(HANDLE)(INT_PTR)(count));
    InvalidateRect(hWnd,NULL,TRUE);
}

int DeselectText(){
    //deselect text if it is selected
    if (isWordSelected){
        isWordSelected = 0;
        startSelectionX = -1;
        startSelectionY = -1;
        endSelectionX = -1;
        endSelectionY = -1;
        return 1;
    }
    return 0;
}

void SetScrolledY(RECT r){
    //will set scrolledy by value of verththumby
    int topY = EDITOR_WRITING_PART_Y - (int)(scrolledY)%(int)(EDITOR_LINE_HEIGHT);

    // int visibleHeight = r.bottom - r.top - EDITOR_WRITING_PART_Y-SCROLL_BAR_WIDTH; //not considered editor_writing_part_y
    int visibleHeight = r.bottom - r.top - topY-SCROLL_BAR_WIDTH; //not considered editor_writing_part_y
    float ratio = visibleHeight/vertThumbHeight;
    scrolledY = ratio*vertThumbY;
}

void SetScrolledX(RECT r){
    //will set scrolledx by value of horzthumbx
    int leftX = EDITOR_WRITING_PART_X - (int)(scrolledX)%(int)(editorFontWidth);

    // int visibleWidth = r.right-r.left-EDITOR_WRITING_PART_X-SCROLL_BAR_WIDTH;
    int visibleWidth = r.right-r.left-leftX-SCROLL_BAR_WIDTH;
    float ratio = visibleWidth/horzThumbWidth;
    scrolledX = ratio*horzThumbX;
}


void DrawScrollBar(HDC hdc,int hx,int hy,int vx,int vy,int width,int height,COLORREF scrollColor,COLORREF thumbColor){
    //x,y are top left coor
    //width and height of scrollbar not thumb
    HBRUSH s = CreateSolidBrush(scrollColor);
    HBRUSH t = CreateSolidBrush(thumbColor);

    RECT scrollV = {vx,vy,vx+SCROLL_BAR_WIDTH,vy+height};
    RECT thumbV = {vx,(int)vertThumbY,vx+SCROLL_BAR_WIDTH,(int)(vertThumbY+vertThumbHeight)};

    RECT scrollH = {hx,hy,hx+width,hy+SCROLL_BAR_WIDTH};
    RECT thumbH = {(int)horzThumbX,hy,(int)(horzThumbX+horzThumbWidth),hy+SCROLL_BAR_WIDTH};

    FillRect(hdc,&scrollV,s);
    FillRect(hdc,&scrollH,s);

    FillRect(hdc,&thumbV,t);
    // DrawTransparentRect(hdc, thumbV,thumbColor , 120);


    FillRect(hdc,&thumbH,t);
    
    DeleteObject(s);
    DeleteObject(t);
}

void removeSelectedCols(std::vector<std::wstring>*content,int sx,int ex,int col){
    //will remove selected cols from a given row
    (*content)[col].erase(sx,ex);
}

std::string getCurrentFilePath(){
    return selectedFilePath;
}

std::vector<std::wstring>& getFileContent(){
    return editContent;
} 

void WriteContentInEditor(std::vector<std::wstring>& content){
    // will write given content in editor and redraw the editor
    editContent = content;
    cursorX = 0;
    cursorY = 0;
    scrolledX = 0;
    scrolledY = 0;
    SetCaretPosition2(cursorX,cursorY);
    InvalidateRect(hEditor,NULL,TRUE);
    SendMessage(hEditor,LOAD_EDITOR_PLUGIN,0,0);
}

void GetCaretPosition(POINT* p){
    //will get caret position
    p->x = cursorX;
    p->y = cursorY;
}

int scrollVerticalParticularLine(int line_no){
    if (cursorY != editContent.size()){
        int topLine = scrolledY/EDITOR_LINE_HEIGHT;
        int k = editContent.size()*EDITOR_LINE_HEIGHT - editorHeight+EDITOR_WRITING_PART_Y-SCROLL_BAR_WIDTH;

        // if (k > 0 && scrolledY + line_no*EDITOR_LINE_HEIGHT < k + ADDITIONAL_SCROLL_LINES){
        if (k > 0 && line_no*EDITOR_LINE_HEIGHT < k + ADDITIONAL_SCROLL_LINES){
            // scrolledY += line_no*EDITOR_LINE_HEIGHT;
            scrolledY = line_no*EDITOR_LINE_HEIGHT;
            cursorX = 0;
            cursorY = line_no;
            SetCaretPosition2(cursorX,cursorY);
        }
        else if(k > 0){
            scrolledY = k + ADDITIONAL_SCROLL_LINES;
        }
        return 1;
    }
    return 0;
}

int scrollDownLine(){
    //will scroll down by one line if cursory is given below certain limit 
    if (cursorY != editContent.size()){
        int topLine = scrolledY/EDITOR_LINE_HEIGHT;
        if ((cursorY+1 - topLine)*EDITOR_LINE_HEIGHT > editorHeight*0.7){
            int k = editContent.size()*EDITOR_LINE_HEIGHT - editorHeight+EDITOR_WRITING_PART_Y-SCROLL_BAR_WIDTH;

            //0.7 -> if we go down beyond 70% screen will also scroll down
            if (k > 0 && scrolledY + EDITOR_LINE_HEIGHT < k + ADDITIONAL_SCROLL_LINES){
                scrolledY += EDITOR_LINE_HEIGHT;
            }
            else if(k > 0){
                scrolledY = k + ADDITIONAL_SCROLL_LINES;
            }
            return 1;
        }
    }
    return 0;
}

int scrollUpLine(){
    //will scroll up if needed
    if (cursorY){
        int topLine = scrolledY/EDITOR_LINE_HEIGHT;
        if ((cursorY-1 - topLine)*EDITOR_LINE_HEIGHT < editorHeight*0.2){
            if (scrolledY - EDITOR_LINE_HEIGHT >= 0){
                scrolledY -= EDITOR_LINE_HEIGHT;
            }
            else{
                scrolledY = 0;
            }
            return 1;
        }
    }
    return 0;
}

int scrollLeftLine(){
    //will scroll left if needed
    if (cursorX) {
        int leftX = scrolledX/editorFontWidth;
        if ((cursorX-1 - leftX)*editorFontWidth < editorWidth*0.1){   
            if (scrolledX - editorFontWidth >= 0){
                scrolledX -= editorFontWidth;
            }
            else{
                scrolledX = 0;
            }
            return 1;
        }
    }
    return 0;
}

int scrollRightLine(){
    if (cursorX != editContent[cursorY].size()){
        int leftX = scrolledX/editorFontWidth;
        if ((cursorX+1-leftX)*editorFontWidth > editorWidth*0.9){
            // if (scrolledX+editorFontWidth < maxLineWidthValue*editorFontWidth - editorWidth + EDITOR_WRITING_PART_X + ADDITIONAL_SCROLL_WIDTH){
            if (scrolledX+editorFontWidth < maxLineWidthValue*editorFontWidth - editorWidth + EDITOR_WRITING_PART_X + ADDITIONAL_SCROLL_WIDTH-SCROLL_BAR_WIDTH){
                scrolledX += editorFontWidth;
            }
            else{
                // scrolledX = maxLineWidthValue*editorFontWidth - editorWidth + EDITOR_WRITING_PART_X  + ADDITIONAL_SCROLL_WIDTH;
                scrolledX = maxLineWidthValue*editorFontWidth - editorWidth + EDITOR_WRITING_PART_X  + ADDITIONAL_SCROLL_WIDTH-SCROLL_BAR_WIDTH;
            }
            return 1;
        }
    }
    return 0;
}

void InsertClipBoardText(std::vector<std::wstring> *content,std::wstring clipboardtext){
    //will insert clipboard text to editor and will set cursor position
    int l = 0;
    int count = 0;
    for (int i = 0;i<clipboardtext.size();i++){
        if (clipboardtext[i] == L'\n'){
            if (content->size() <= cursorY) content->push_back(L"");
            content->insert(content->begin()+cursorY,clipboardtext.substr(l,i-l));
            l = i+1;
            cursorY++;
        }
    }
    if (clipboardtext[clipboardtext.size()-1] != L'\n') {
        int k = (*content)[cursorY].size();
        (*content)[cursorY].insert(0,clipboardtext.substr(l));
        // (*content)[cursorY].insert(0,SUBSTR(clipboardtext,l,clipboardtext.size(),__FILE__,__LINE__));
        cursorX = k;
    }
    else cursorX = l;
    SetCaretPosition2(cursorX,cursorY);
}

std::wstring copySelectedText(std::vector<std::wstring> *content, int sx,int sy,int ex,int ey){
    //will return selected text
    std::wstring buffer = L"";

    if (!(*content).size() || sx < 0 || ex < 0 || sy < 0 || ey < 0) return L" ";
    if (sy != ey){
        buffer += (*content)[sy].substr(sx) + L"\n";
        // buffer += (*content)[sy].substr(sx) + L"\n";
        for(int i = sy+1;i<ey;i++) buffer += (*content)[i] + L"\n";
        buffer += (*content)[ey].substr(0,ex);
    }
    else {
        // content->push_back((*content)[sy].substr(sx));
        buffer += (*content)[sy].substr(sx,ex-sx);
    }
    // else content->push_back(SUBSTR((*content)[sy],sx,(*content)[sy].size(),__FILE__,__LINE__));
    
    return buffer;
}

std::wstring GetClipboardText() {
    //will get text of clipboard
    if (!OpenClipboard(NULL)) return L"";

    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (!hData) {
        CloseClipboard();
        return L"";
    }

    wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
    if (!pszText) {
        CloseClipboard();
        return L"";
    }

    std::wstring text(pszText); 
    GlobalUnlock(hData);
    CloseClipboard();
    return text;
}


void setClipBoardText(const std::wstring &text){
    //will set text to clipboard
    
    if (!OpenClipboard(NULL)) return ;

    EmptyClipboard();


    size_t sizeInBytes = (text.length() + 1) * sizeof(wchar_t);
    HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, sizeInBytes);
    if (!hGlob) {
        CloseClipboard();
        return;
    }


    memcpy(GlobalLock(hGlob), text.c_str(), sizeInBytes);
    GlobalUnlock(hGlob);
    SetClipboardData(CF_UNICODETEXT, hGlob);

    CloseClipboard();
}


void removeSelectedText(std::vector<std::wstring>*content,int sx,int sy,int ex,int ey){
    //will remove the selected text
    if (sx < 0 || sy < 0 || ex < 0 || ey < 0) return ;
    if (startSelectionX > endSelectionX) swap(&startSelectionX,&endSelectionX);
    if (startSelectionY > endSelectionY) swap(&startSelectionY,&endSelectionY);
    
    //will copy deleted text
    std::wstring copyText;
    if (isWordSelected) copyText = copySelectedText(&editContent,startSelectionX,startSelectionY,endSelectionX,endSelectionY);
    else copyText = editContent[cursorY] + L"\n"; //line selected
    setClipBoardText(copyText);
    
    if (sy != ey){
        std::wstring before = (*content)[sy].substr(0, sx);
        (*content)[sy].erase(sx);
        std::wstring after = (*content)[ey].substr(ex);
        (*content)[ey].erase(0,ex);

        (*content)[sy] = before + after;
        if (!(*content)[sy].size()) content->erase(content->begin()+sy,content->begin()+ey+1);
        else content->erase(content->begin()+sy+1,content->begin()+ey+1);
    }
    else (*content)[sy].erase(sx,ex-sx);
    isWordSelected = 0;
}

void scrollRightMostPart(int x,int y){
    //will set scrollx to given lines ends
    int visible = (editorWidth-EDITOR_WRITING_PART_X-SCROLL_BAR_WIDTH)/editorFontWidth;
    if (x > visible)
    scrolledX = (x-visible)*editorFontWidth + ADDITIONAL_SCROLL_WIDTH;
    if (scrolledY > EDITOR_LINE_HEIGHT) scrolledY -= EDITOR_LINE_HEIGHT; //scroll only if needed
    else scrolledY = 0;
}

int clearSelection(HWND hWnd){
    //will use removeSelectedText to remove and set cursor position
    if (isWordSelected){
        //may need to move screen
        cursorX = startSelectionX;
        cursorY = startSelectionY;
        removeSelectedText(&editContent,startSelectionX,startSelectionY,endSelectionX,endSelectionY);
        startSelectionX = -1;
        startSelectionY = -1;
        endSelectionX = -1;
        endSelectionY = -1;
        SetCaretPosition2(cursorX,cursorY);
        InvalidateRect(hWnd,NULL,TRUE);
        return 1;
    }
    return 0;
}

void deleteAutoCompleteWindow(){
    //will destroy auto complete window
    if (hautoComplete){
        // plugin.clear();
        DestroyWindow(hautoComplete);
        hautoComplete = NULL;
    }
}

// void WriteFileContent(std::string path){
//     //will write content of file in editor
//     std::wifstream inpfile(path);
//     // inpfile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>)); // if UTF-8 file

//     if (inpfile.is_open()){
//         std::wstring line;
//         editContent.clear();
//         scrolledY = 0;
//         scrolledX = 0;
//         cursorX = 0;
//         cursorY = 0;
//         while (std::getline(inpfile,line)){
//             editContent.push_back(line);
//         }
//         SetCaretPosition2(cursorX,cursorY);
//     }
//     inpfile.close();

// }


LANGUAGE_TYPE getCurrentLanguage(){
    if (explorer.selected_project){
        // std::string file_name = explorer.selected_project->name;
        std::string file_name = selectedFilePath;
        int pos = -1;
        for (int i = file_name.size()-1;i>=0;i--) if (file_name[i] == '.'){pos = i;break;}
        if (pos >= 0) {
            std::string type = file_name.substr(pos + 1);
            // if (type == "h" || type == "cpp") return LANGUAGE_TYPE::PYTHON;
            if (type == "h" || type == "cpp" || type == "c") return LANGUAGE_TYPE::C;
            else if (type == "json") return LANGUAGE_TYPE::JSON;
            else if (type == "text") return LANGUAGE_TYPE::TEXT; 
            else if (type == "py") return LANGUAGE_TYPE::PYTHON;

        }
    }
    return LANGUAGE_TYPE::TEXT;
}

// void load_plugin(std::wstring code,int cx,int cy){
    // plugin.init("python");

//     // std::wstring code = combineLineContent(editContent,0,editContent.size());
//     // plugin.load(wstring_string(code),cursorX,cursorY);
//     plugin.load(wstring_string(code),cx,cy);
// }

// void load_plugin_safe(std::vector<std::wstring> contentCopy, int cx, int cy) {
//     int my_id = ++plugin_task_id;

//     std::thread([=]() {
//         // std::this_thread::sleep_for(std::chrono::milliseconds(30)); // debounce buffer
//         if (my_id != plugin_task_id.load()) {
//             return;
//         }

//         std::lock_guard<std::mutex> lock(plugin_mutex); // prevent overlap

//         if (my_id != plugin_task_id.load()) {
//             return;
//         }

//         try {

//             // plugin.init("python");
//             plugin.init(getCurrentLanguage());

//             std::wstring code = combineLineContent(contentCopy, 0, contentCopy.size());

//             if (my_id != plugin_task_id.load()) {
//                 return;
//             }

//             plugin.load(wstring_string(code), cx, cy);
  
//             std::cout << "loaded ";
//         } 
//         catch (...) {
//             ;
//         }
//         std::cout << "lol ";
//         if (hEditor){
//             InvalidateRect(hEditor, NULL, TRUE);
//             SendMessage(hEditor,AUTO_COMPLETE_CREATE,0,0);
//             std::cout << " running " << my_id;

//         }

//     }).detach();

// }


void load_plugin_safe(std::vector<std::wstring> contentCopy, int cx, int cy) {
    int my_id = ++plugin_task_id;

    std::thread([my_id, contentCopy = std::move(contentCopy), cx, cy]() {
        // Wait a bit to coalesce rapid calls
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        if (my_id != plugin_task_id.load()) return;

        {
            std::lock_guard<std::mutex> lock(plugin_mutex);

            if (my_id != plugin_task_id.load()) return;

            try {
                plugin.init(getCurrentLanguage());

                std::wstring code = combineLineContent(contentCopy, 0, contentCopy.size());

                if (my_id != plugin_task_id.load()) return;

                plugin.load(wstring_string(code), cx, cy);

            } catch (...) {
                ;
            }
        }

        // Outside the lock, UI updates
        if (my_id == plugin_task_id.load() && hEditor) {
            InvalidateRect(hEditor, NULL, TRUE);
            SendMessage(hEditor, AUTO_COMPLETE_CREATE, 0, 0);
        }

    }).detach();
}

// void initialize_char_colors(std::vector<std::wstring>* content,int startLine,int endLine,int topY,int maxWidth,int startX,int endX,int leftX){
    // int index = 0;
    // for (int y = startLine;y < content->size() && y < endLine;y++){
        // if (allcharcolor.size() >= y && (*content)[y].size() == allcharcolor[y].size()){
            // for (int x = startX;x < endX && x < (*content)[y].size();x++){
                // if ()
            // }
        // }
    // }

// }














// void WriteContentInEditor(HDC hdc,std::vector<std::wstring>* content,int startLine,int endLine,int topY,int maxWidth,int startX,int endX,int leftX){
//     SelectObject(hdc, editorFont);
//     SetBkMode(hdc, TRANSPARENT);
//     SetTextColor(hdc, colorTheme.editortc);

//     InvalidateRect(hminiCodeViewer,NULL,TRUE);

//     if (!(*content).size()) content->push_back(L"");

//     int s = 0;

//     if (isselectingWord | isWordSelected){
//         if (startSelectionX > endSelectionX) swap(&startSelectionX,&endSelectionX);
//         if (startSelectionY > endSelectionY) swap(&startSelectionY,&endSelectionY);
//         if (startSelectionX >=0 && startSelectionY >= 0 && endSelectionX >= 0 && endSelectionY >= 0) s = 1;
//     }

//     // std::wstring code = combineLineContent(editContent,0,editContent.size());
//     std::wstring code = combineLineContent(*content,0,content->size());

//     // plugin.load(wstring_string(code),cursorX,cursorY);
//     // plugin.init("text");
//     plugin.init("python");
//     // std::thread pluginThread([=]() {
//         // plugin.load(wstring_string(code), cursorX, cursorY, startX, startLine, endX, endLine);
//     // });
//     // pluginThread.detach();  // Detached safely since all values are copied
    
//     // plugin.load(wstring_string(code),cursorX,cursorY,startX,startLine,endX,endLine);
    
//     // plugin.load(wstring_string(code),cursorX,cursorY);
//     plugin.display(startX,startLine,endX,endLine);

//     // plugin.display(startX,startLine,endX,endLine);

//     // if (plugin.plugin.current_string == "") deleteAutoCompleteWindow();

//     // std::vector<TokenStyle*> t;
//     // t = *plugin.styleDisplayTokens;

//     int index = 0;

//     for (int y = startLine;y < (*content).size() && y < endLine;y++){
//         int drawY = (y-startLine)*EDITOR_LINE_HEIGHT + topY;
//         std::wstring line = (*content)[y];
//         // if (plugin.display_tokens.size() && plugin.display_tokens[index]->startLine != y){
        
//         if (index < plugin.display_tokens->size() && plugin.display_tokens->size() && (*plugin.display_tokens)[index]->startLine != y){
//             // for (int p = index;p<plugin.display_tokens.size();p++) {
//             for (int p = index;p<plugin.display_tokens->size();p++) {
//                 // if (plugin.display_tokens[p]->startLine == y){
//                 if ((*plugin.display_tokens)[p]->startLine <= y && y <= (*plugin.display_tokens)[p]->endLine){
//                 // if ((*plugin.display_tokens)[p]->startLine == y || (y > (*plugin.display_tokens)[p]->startLine && y <= (*plugin.display_tokens)[p]->endLine)){
//                     index = p;
//                     std::cout << index << " ";
//                     // std::cout << "index : "<<index << "\n";
//                     break;
//                 }
//             }
//         }
//         for (int x = startX;x < endX && x < line.size();x++){
//             int drawX = (x-startX)*editorFontWidth + leftX;


//             if (line[x] == L' ' || line[x] == L'\n'){
//                 SetTextColor(hdc, colorTheme.editortc);
//             }
//             // else if (index < plugin.display_tokens.size() && x >= plugin.display_tokens[index]->start && x < plugin.display_tokens[index]->end){
//             // else if (index < plugin.display_tokens->size() && x >= (*plugin.display_tokens)[index]->start && x < (*plugin.display_tokens)[index]->end){
//             // else if (index < plugin.display_tokens->size() && ((x >= (*plugin.display_tokens)[index]->start && x < (*plugin.display_tokens)[index]->end && (*plugin.display_tokens)[index]->startLine == (*plugin.display_tokens)[index]->endLine))){
//                 // SetTextColor(hdc, plugin.display_tokens[index]->color);
//             else if (index <  plugin.display_tokens->size()){
//                 // std::cout << "line : " << (*plugin.display_tokens)[index]->startLine << " " << (*plugin.display_tokens)[index]->endLine << "\n";
//                 if ((*plugin.display_tokens)[index]->startLine == (*plugin.display_tokens)[index]->endLine && x >= (*plugin.display_tokens)[index]->start && x < (*plugin.display_tokens)[index]->end){
//                     SetTextColor(hdc, (*plugin.display_tokens)[index]->color);
//                     std::cout << "hello world";
//                 }
//                 else if ((*plugin.display_tokens)[index]->startLine != (*plugin.display_tokens)[index]->endLine){
//                     if ((y == (*plugin.display_tokens)[index]->startLine && x >= (*plugin.display_tokens)[index]->start) || (y == (*plugin.display_tokens)[index]->endLine && x < (*plugin.display_tokens)[index]->end) || (y > (*plugin.display_tokens)[index]->startLine && y < y == (*plugin.display_tokens)[index]->endLine)){
//                         SetTextColor(hdc, (*plugin.display_tokens)[index]->color);
//                     }
//                     else SetTextColor(hdc, colorTheme.editortc);

//                 }
//                 else 
//                     SetTextColor(hdc, colorTheme.editortc);

//             }
//             else{
//                 index++;
//                 // if (plugin.display_tokens.size() && index < plugin.display_tokens.size() && x >= plugin.display_tokens[index]->start && x < plugin.display_tokens[index]->end) {
//                 if (plugin.display_tokens->size() && index < plugin.display_tokens->size() && x >= (*plugin.display_tokens)[index]->start && x < (*plugin.display_tokens)[index]->end) {
//                     // SetTextColor(hdc, plugin.display_tokens[index]->color);
//                     SetTextColor(hdc, (*plugin.display_tokens)[index]->color);
//                 }
//                 else{
//                     SetTextColor(hdc, colorTheme.editortc);
//                 }

//             }

//             if (s){
//                 if (startSelectionY == endSelectionY){
//                     if (y == startSelectionY && x >= startSelectionX && x < endSelectionX){
//                         SetBkMode(hdc, OPAQUE);  // background drawing
//                         SetBkColor(hdc, colorTheme.editorSelectedTextbk); // blue highlight
//                         TextOut(hdc, drawX, drawY, &line[x], 1);
//                         SetBkMode(hdc, TRANSPARENT);  // background drawing
//                         // SetTextColor(hdc, colorTheme.editortc);
//                     }
//                     else TextOut(hdc, drawX, drawY, &line[x], 1);

//                 }

//                 else if ((startSelectionY == endSelectionY && y == startSelectionY && x >= startSelectionX && x < endSelectionX) || (y == startSelectionY && x >= startSelectionX) || (y == endSelectionY && x < endSelectionX) || (y > startSelectionY && y < endSelectionY)){
//                     SetBkMode(hdc, OPAQUE);  // background drawing
//                     SetBkColor(hdc, colorTheme.editorSelectedTextbk); // blue highlight
//                     TextOut(hdc, drawX, drawY, &line[x], 1);
//                     SetBkMode(hdc, TRANSPARENT);  // background drawing
//                     // SetTextColor(hdc, colorTheme.editortc);
//                 }
//                 else TextOut(hdc, drawX, drawY, &line[x], 1);

//             }
//             else TextOut(hdc, drawX, drawY, &line[x], 1);
//         }
//     }

// }


void draw_line_no(HDC hdc,int startLineNo,int lastLineNo,int topY){
    // draws line_no
    // SetTextColor(hdc,RGB(100,100,100));
    SetTextColor(hdc,colorTheme.editortc);
    for (int i = startLineNo;i<lastLineNo && i < editContent.size();i++){
        int drawY = (i-startLineNo)*EDITOR_LINE_HEIGHT + topY;
        const wchar_t* wstr = std::to_wstring(i+1).c_str();
        size_t l = wcslen(wstr);
        int d = EDITOR_WRITING_PART_X;
        int k = (d - l*editorFontWidth)/2;
        TextOut(hdc,k,drawY,wstr,wcslen(wstr));
    }
}




void WriteContentInEditor(HDC hdc,std::vector<std::wstring>* content,int startLine,int endLine,int topY,int maxWidth,int startX,int endX,int leftX){
    SelectObject(hdc, editorFont);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, colorTheme.editortc);

    // InvalidateRect(hminiCodeViewer,NULL,TRUE);
//     if (!hhover){
//     createHoverWindow_(
//     L"This is a paragraph.\nIt wraps automatically and supports default font.",
//     300, 100, 300, 100,
//     hmainWindow,
//     RGB(30, 30, 30),       // background color
//     RGB(220, 220, 220)     // text color
// );
    // }


    // ShowHoverWindow(L"This is a hover panel.\nText is selectable and copyable.", 300, 200, 300, 100, hmainWindow/* your parent */);

    // TextDisplayWindow myTextWindow(hmainWindow,200, 200, 300, 150,RGB(0,0,0),RGB(200,200,200));
    // myTextWindow.setText(L"Hello, World!\nThis is your custom text window.");
    // myTextWindow.show();

//     showHoverText(
//     L"This is selectable & copyable text.\nClick outside to dismiss.",
//     500, 300, 300, 100,hEditor
// );

    if (!(*content).size()) content->push_back(L"");

    int s = 0;

    if (isselectingWord | isWordSelected){
        // if (startSelectionX > endSelectionX) swap(&startSelectionX,&endSelectionX);
        // if (startSelectionY > endSelectionY) swap(&startSelectionY,&endSelectionY);
        if (startSelectionX >=0 && startSelectionY >= 0 && endSelectionX >= 0 && endSelectionY >= 0) s = 1;
    }

    std::wstring code = combineLineContent(*content,0,content->size());

    // plugin.init("python");
    plugin.init(getCurrentLanguage());
 
    plugin.display(startX,startLine,endX,endLine);

    int index = 0;

    for (int y = startLine;y < (*content).size() && y < endLine;y++){
        int drawY = (y-startLine)*EDITOR_LINE_HEIGHT + topY;
        std::wstring line = (*content)[y];
        
        for (int p = index;p<plugin.display_tokens->size();p++) {
            if ((*plugin.display_tokens)[p]->startLine <= y && y <= (*plugin.display_tokens)[p]->endLine){
                index = p;
                break;
            }
        }

        for (int x = startX;x < endX && x < line.size();x++){
            int drawX = (x-startX)*editorFontWidth + leftX;

            if (line[x] == L' ' || line[x] == L'\n'){
                SetTextColor(hdc, colorTheme.editortc);
            }
            else if (index <  plugin.display_tokens->size()){
                if ((*plugin.display_tokens)[index]->startLine == (*plugin.display_tokens)[index]->endLine && x >= (*plugin.display_tokens)[index]->start && x < (*plugin.display_tokens)[index]->end){
                    SetTextColor(hdc, (*plugin.display_tokens)[index]->color);
                }
                else if ((*plugin.display_tokens)[index]->startLine != (*plugin.display_tokens)[index]->endLine){
                    if ((y == (*plugin.display_tokens)[index]->startLine && x >= (*plugin.display_tokens)[index]->start) || (y == (*plugin.display_tokens)[index]->endLine && x < (*plugin.display_tokens)[index]->end) || (y > (*plugin.display_tokens)[index]->startLine && y < (*plugin.display_tokens)[index]->endLine)){
                        SetTextColor(hdc, (*plugin.display_tokens)[index]->color);
                    }
                    else {
                        if (index + 1 < plugin.display_tokens->size()){
                            index++;
                            SetTextColor(hdc, (*plugin.display_tokens)[index]->color);
                        }
                        else SetTextColor(hdc, colorTheme.editortc);
                    }

                }
                else {
                    if (index + 1 < plugin.display_tokens->size()){
                        index++;
                        SetTextColor(hdc, (*plugin.display_tokens)[index]->color);
                    }
                    else

                    // continue;
                    SetTextColor(hdc, colorTheme.editortc);
                }

            }
            else SetTextColor(hdc, colorTheme.editortc);

            if (s){
                if (startSelectionY == endSelectionY){
                    if (y == startSelectionY && x >= startSelectionX && x < endSelectionX){
                        SetBkMode(hdc, OPAQUE);  // background drawing
                        SetBkColor(hdc, colorTheme.editorSelectedTextbk); // blue highlight
                        TextOut(hdc, drawX, drawY, &line[x], 1);
                        SetBkMode(hdc, TRANSPARENT);  // background drawing
                    }
                    else TextOut(hdc, drawX, drawY, &line[x], 1);

                }

                else if ((startSelectionY == endSelectionY && y == startSelectionY && x >= startSelectionX && x < endSelectionX) || (y == startSelectionY && x >= startSelectionX) || (y == endSelectionY && x < endSelectionX) || (y > startSelectionY && y < endSelectionY)){
                    SetBkMode(hdc, OPAQUE);  // background drawing
                    SetBkColor(hdc, colorTheme.editorSelectedTextbk); // blue highlight
                    TextOut(hdc, drawX, drawY, &line[x], 1);
                    SetBkMode(hdc, TRANSPARENT);  // background drawing
                    // SetTextColor(hdc, colorTheme.editortc);
                }
                else TextOut(hdc, drawX, drawY, &line[x], 1);

            }
            else TextOut(hdc, drawX, drawY, &line[x], 1);
        }
    }

}







#endif