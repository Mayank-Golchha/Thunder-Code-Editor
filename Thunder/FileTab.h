// // TODO : handle resizing
// // TODO : handle horizontal scrolling
// // TODO : file name width cannot be more than file tab width 
// // if tabs are more than the width then the tab will not be displayed

// #ifndef FILETAB_H
// #define FILETAB_H

// #include <vector>

// #include "colors.h"
// #include "window_handles.h"
// #include "string_operations.h"
// #include "dimensions.h"
// #include "font.h"
// #include "customWindow.h"
// #include "drawing.h"
// #include "open_file_tabs.h"

// // int activeFileTabId = -1;
// // int totalTabs = 0;

// // struct FileTab {
// //     HWND handle;
// //     std::string FileName;
// //     std::string FilePath;
// //     HFONT font;
// //     int hover;
// //     int id;
// //     int x;
// // };

// // std::vector<FileTab*> fileTabs;
// // HWND hoverWindowFilePath;

// struct FileTab* a = new FileTab{ NULL,"Computer","asd",NULL,0 ,-1,-1};
// struct FileTab* b = new FileTab{ NULL,"World","asd",NULL,0 ,-1,-1};
// struct FileTab* c = new FileTab{ NULL,"hi","asd",NULL,0 ,-1,-1};
// struct FileTab* d = new FileTab{ NULL,"i am","asd",NULL,0 ,-1,-1};
// struct FileTab* e = new FileTab{ NULL,"Mayank","asd",NULL,0 ,-1,-1};
// struct FileTab* f = new FileTab{ NULL,"Golchha","asd",NULL,0 ,-1,-1};
// struct FileTab* g = new FileTab{ NULL,"a","asd",NULL,0 ,-1,-1};
// struct FileTab* hi = new FileTab{ NULL,"Computer","asd",NULL,0,-1,-1};
// struct FileTab* i = new FileTab{ NULL,"Science","asd",NULL,0,-1,-1};

// HWND createFileTabContainer(HWND);

// LRESULT CALLBACK FileTabContainerProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
//     WNDPROC oldProc = (WNDPROC)GetProp(hWnd, L"OldContainerProc");

//     if (!oldProc)
//         return DefWindowProc(hWnd, msg, wp, lp);

//     switch (msg) {
//         case WM_PAINT: {
//             PAINTSTRUCT ps;
//             HBRUSH brush = CreateSolidBrush(colorTheme.filetabContainer);
//             HDC hdc = BeginPaint(hWnd, &ps);

//             RECT rc;
//             GetClientRect(hWnd, &rc);

//             FillRect(hdc, &rc, brush);

//             DeleteObject(brush);
//             EndPaint(hWnd, &ps);
//             return 0;
//         }
//     }
//     return CallWindowProcW(oldProc, hWnd, msg, wp, lp);


//     // return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA), hWnd, msg, wp, lp);
// }


// LRESULT CALLBACK FileTabProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
//     HFONT font = customFont(FILE_TAB_FONT_SIZE, 8, FILE_TAB_FONT_WEIGHT, L"Segoe UI");

//     FileTab* data = (FileTab*)GetProp(hWnd, L"FileTabPtr");
//     WNDPROC oldProc = (WNDPROC)GetProp(hWnd, L"OldProc");

//     if (!oldProc)
//         return DefWindowProc(hWnd, msg, wp, lp);
    
//     switch (msg) {
        
//         case WM_MOUSEMOVE:
//             if (!data->hover) {
//                 TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
//                 tme.dwFlags = TME_HOVER | TME_LEAVE;
//                 tme.hwndTrack = hWnd;
//                 tme.dwHoverTime = HOVER_DEFAULT;
//                 // tme.dwHoverTime = TME_HOVER; //very fast hovering
//                 TrackMouseEvent(&tme);
//                 data->hover = 1;
//                 InvalidateRect(hWnd,NULL,TRUE);
//             }
//             return 0;
        
//         case WM_MOUSEHOVER:
//             {
//                 HFONT font = customFont(15, 6, FILE_TAB_FONT_WEIGHT, L"Segoe UI");
//                 std::wstring wtext = string_to_wstring(data->FilePath);
//                 const wchar_t* text = wtext.c_str();
//                 int width = calculateFontWidth(text, font);

//                 hoverWindowFilePath = customWindow(text,data->x + FILE_TAB_WIDTH/3,FILE_TAB_Y + FILE_TAB_HEIGHT,width,FILE_TAB_HOVER_WINDOW_HEIGHT,
//                     hmainWindow,colorTheme.hoverWindowColorbk,colorTheme.hoverWindowColortc,0,0,font);

//                 return 0;
//             }

//         case WM_MOUSELEAVE:
//             data->hover = 0;
//             InvalidateRect(hWnd,NULL,TRUE);
//             DestroyWindow(hoverWindowFilePath);

//             return 0;

//         case WM_LBUTTONDOWN: 
//         {
//             if (fileTabs.empty()) return 0;
//             POINT p;
//             GetCursorPos(&p);
//             ScreenToClient(hWnd, &p);

//             RECT r;
//             GetClientRect(hWnd, &r);

//             if (p.x > r.right - FILE_TAB_CLOSE_BUTTON_WIDTH) {
//                 if (data->id == activeFileTabId) activeFileTabId = -1;

//                 else if (activeFileTabId > data->id) activeFileTabId--;

//                 fileTabs.erase(fileTabs.begin() + data->id);

//                 DestroyWindow(hWnd);
//                 DestroyWindow(hoverWindowFilePath);

//                 DestroyWindow(hFileTabContainer);
//                 hFileTabContainer = createFileTabContainer(hmainWindow);
                
//                 return 0;
//             }
//             else {
//                 HWND old;
//                 if (activeFileTabId >= 0){
//                     old = fileTabs[activeFileTabId]->handle;
//                 }
//                 activeFileTabId = data->id;
//                 if (old) InvalidateRect(old,NULL,TRUE);
//                 InvalidateRect(hWnd,NULL,TRUE);
//             }

//             // break;
//             return 0;
//         }

//         case WM_PAINT:
//             {
//             PAINTSTRUCT ps;
//             HBRUSH brush;
//             if (activeFileTabId == data->id) brush = CreateSolidBrush(colorTheme.filetabhoverbk);
//             else brush = CreateSolidBrush(colorTheme.filetabbk);
//             HDC hdc = BeginPaint(hWnd, &ps);

//             RECT rc;
//             GetClientRect(hWnd, &rc);

//             HFONT font = customFont(FILE_TAB_FONT_SIZE, 8, FILE_TAB_FONT_WEIGHT, L"Segoe UI");

//             int k = (rc.bottom - FILE_TAB_HEIGHT) / 2 + 5;

//             FillRect(hdc, &rc, brush);
        

//             SelectObject(hdc, font);
//             SetBkMode(hdc, TRANSPARENT);
//             if (activeFileTabId == data->id) SetTextColor(hdc, colorTheme.filetabhovertc);
//             else SetTextColor(hdc, colorTheme.filetabtc);

//             wchar_t buf[256];
//             GetWindowTextW(hWnd, buf, 256);

//             TextOut(hdc, 8, k, buf, wcslen(buf)); // file name
//             if (data->hover){
//                 // POINT pp;
//                 // GetCursorPos(&pp);
//                 // ScreenToClient(hWnd,&pp);
//                 TextOut(hdc, rc.right - 20, k, L"X", wcslen(L"X"));
//                 // if (pp.y > 0)
//                     // DrawGradientLine(hdc, 0,pp.y, 0, RGB(11, 7, 235), RGB(235, 7, 231));

//             }

//             if (activeFileTabId == data->id){
//                 DrawGradientLine(hdc, 0, rc.right, 0, RGB(11, 7, 235), RGB(235, 7, 231));

//                 // DrawLine(hdc,0,0,rc.right,0,2,colorTheme.hoverwindowhighlightline);
//             }

//             DeleteObject(brush);
//             DeleteObject(font);
//             EndPaint(hWnd, &ps);
//             return 0;
//         }
    

//         case WM_CREATE:
//             SendMessage(hWnd, WM_SETFONT, (WPARAM)font, TRUE);
//             // break;
//             return 0;


//         // case WM_SETFONT:
//             // if (LOWORD(lp)) InvalidateRect(hWnd, NULL, TRUE);
//             // return 0;

//         case WM_DESTROY:
//             RemoveProp(hWnd, L"FileTabPtr");
//             // break;
//             return 0;

//     }
//     return CallWindowProcW(oldProc, hWnd, msg, wp, lp);


//     // return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA), hWnd, msg, wp, lp);
// }

// HWND createFileTab(HWND hWnd, int x, int y, int width, int height, const wchar_t* text, int Id) {
//     HWND tab = CreateWindowEx(0, L"static", text, WS_CHILD | WS_VISIBLE | SS_NOTIFY,
//         x, y, width, height, hWnd, (HMENU)(INT_PTR)Id, NULL, NULL);
    
//     WNDPROC oldProc = (WNDPROC)SetWindowLongPtrW(tab, GWLP_WNDPROC, (LONG_PTR)FileTabProc);
//     SetProp(tab, L"OldProc", (HANDLE)oldProc);
//     // SetWindowLongPtrW(tab, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtrW(tab, GWLP_WNDPROC, (LONG_PTR)FileTabProc));

//     SetProp(tab, L"FileTabPtr", (HANDLE)fileTabs[Id]);

//     return tab;
// }

// HWND createFileTabContainer(HWND hWnd) {
//     RECT r;
//     GetClientRect(hWnd, &r);
//     //adding 14 because r.right because was getting wrong width
//     //dont know why
//     int w = r.right + 14;

//     hFileTabContainer = CreateWindowEx(0, L"static", L"", WS_CHILD | WS_VISIBLE,
//         FILE_TAB_X, FILE_TAB_Y, w, FILE_TAB_HEIGHT, hWnd, NULL, NULL, NULL);
    
//     WNDPROC oldProc = (WNDPROC)SetWindowLongPtrW(hFileTabContainer, GWLP_WNDPROC, (LONG_PTR)FileTabContainerProc);
//     SetProp(hFileTabContainer, L"OldContainerProc", (HANDLE)oldProc);
//     // SetWindowLongPtrW(hFileTabContainer, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtrW(hFileTabContainer, GWLP_WNDPROC, (LONG_PTR)FileTabContainerProc));
//     for (int i = 0; i < fileTabs.size(); i++) {
//         DestroyWindow(fileTabs[i]->handle);
//     }


//     HFONT font = customFont(FILE_TAB_FONT_SIZE, 8, FILE_TAB_FONT_WEIGHT, L"Segoe UI");

//     // int temp = FILE_TAB_X;
//     int temp = 0;

//     for (int i = 0; i < fileTabs.size(); i++) {
//         FileTab* f = fileTabs[i];
//         std::wstring wtext = string_to_wstring(f->FileName);
//         const wchar_t* text = wtext.c_str();
//         int width = calculateFontWidth(text, font);
//         // int width = FILE_TAB_WIDTH;
//         if (temp + width >= w) break;

//         HWND tab = createFileTab(hFileTabContainer, temp, 0, width, FILE_TAB_HEIGHT, text, i);
//         fileTabs[i]->handle = tab;
//         fileTabs[i]->id = i;
//         // fileTabs[i]->x = temp;
//         fileTabs[i]->x = FILE_TAB_X + temp;
//         temp += width + 1;
//     }


//     DeleteObject(font);
//     return hFileTabContainer;
// }

// #endif
















// TODO : handle resizing
// TODO : handle horizontal scrolling
// TODO : file name width cannot be more than file tab width 
// TODO : free memory of filetabs
// if tabs are more than the width then the tab will not be displayed

#ifndef FILETAB_H
#define FILETAB_H

#include <vector>
// #include <fstream>

#include "colors.h"
#include "window_handles.h"
#include "string_operations.h"
#include "dimensions.h"
#include "font.h"
#include "customWindow.h"
#include "drawing.h"
#include "open_file_tabs.h"
#include "editor_resources.h"
// #include "file_explorer.h"

// int activeFileTabId = -1;
// int totalTabs = 0;

// struct FileTab {
//     HWND handle;
//     std::string FileName;
//     std::string FilePath;
//     HFONT font;
//     int hover;
//     int id;
//     int x;
// };

// std::vector<FileTab*> fileTabs;
// HWND hoverWindowFilePath;


// struct FileTab* a = new FileTab{ NULL,"Computer","asd",NULL,0 ,-1,-1};
// struct FileTab* b = new FileTab{ NULL,"World","asd",NULL,0 ,-1,-1};
// struct FileTab* c = new FileTab{ NULL,"hi","asd",NULL,0 ,-1,-1};
// struct FileTab* d = new FileTab{ NULL,"i am","asd",NULL,0 ,-1,-1};
// struct FileTab* e = new FileTab{ NULL,"Mayank","asd",NULL,0 ,-1,-1};
// struct FileTab* f = new FileTab{ NULL,"Golchha","asd",NULL,0 ,-1,-1};
// struct FileTab* g = new FileTab{ NULL,"a","asd",NULL,0 ,-1,-1};

void writeOpenFileTabsInFile(std::vector<FileTab*> tabs){
    // write all the open file tabs in the file when closing the app
    std::string file_path = ".thunder/config/recent_file_tabs.bin";
    std::ofstream file(file_path,std::ios::binary | std::ios::out);
    
    if (!file.is_open()) {
        logger.warning_log("Unable to Open Recent File Tabs File");
        return;
    }

    size_t num_elements = tabs.size();
    file.write(reinterpret_cast<const char*>(&num_elements), sizeof(num_elements));

    for (int i = 0;i<tabs.size();i++){
        file.write(reinterpret_cast<char*>(tabs[i]),sizeof(FileTab));
    }

    file.close();
}

std::vector<FileTab*> readOpenFileTabsFromFile(std::vector<FileTab*>& tabs){
    // read the recent opened file tabs when starting the application for the first time
    std::string file_path = ".thunder/config/recent_file_tabs.bin";
    std::ifstream file(file_path,std::ios::binary | std::ios::in);

    if (!file.is_open()) {
        logger.warning_log("Unable to Open Recent File Tabs File");
        return {};
    }

    size_t n;
    file.read(reinterpret_cast<char*>(&n), sizeof(n));

    std::cout << n << " ";

    for (int i = 0;i<n;i++){
        FileTab* f = new FileTab();
        file.read(reinterpret_cast<char*>(f), sizeof(FileTab));
        tabs.push_back(f);
    }

    file.close();

    return tabs;
}

HWND createFileTabContainer(HWND);

class FileTabContainer{
    public:
        int selectedIndex = -1;
        FileTab* selectedTab = NULL;
        std::vector<FileTab*> file_tabs;
        int hover = -1;
        int scrolled = 0;
        HFONT font = customFont(FILE_TAB_FONT_SIZE, 8, FILE_TAB_FONT_WEIGHT, L"Segoe UI");
        std::vector<int> tabWidths; //will save tab widths

        FileTabContainer(){
            // fileTabs = fileTabs_;
            // file_tabs.push_back(a);
    // file_tabs.push_back(b);
    // file_tabs.push_back(c);
    // file_tabs.push_back(d);
    // file_tabs.push_back(e);
    // file_tabs.push_back(f);
    // file_tabs.push_back(g);
        }

        void drawFileTab(HDC hdc,std::string file_name,int x,int y,int width,int height,COLORREF bkcolor, COLORREF txtcolor,bool event = false){
            //event is true for hover or select
            HBRUSH bg = CreateSolidBrush(bkcolor);
            RECT r = {x,y,x+width,y + height};
            FillRect(hdc, &r, bg);
            SetBkMode(hdc, TRANSPARENT);  // background drawing
            SetTextColor(hdc,txtcolor);
            r.left = x;
            std::wstring wstr = string_to_wstring(file_name);
            TextOut(hdc, x + 10 , 4 ,wstr.c_str(), wcslen(wstr.c_str()));
            if (event) TextOut(hdc, x + width - 20, 4, L"X", wcslen(L"X"));
            // tabWidths.push_back(width);
            DeleteObject(bg);
        }

        int findHoverIndex(int x_){
            //given x_ is mouse x
            //will find the index of tab the mouse is on 
            x_ += scrolled;
            int temp = 0;
            for (int i = 0;i<tabWidths.size();i++){
                if (temp + i < x_ && x_ < tabWidths[i] + i+1) return i;
                // if (temp < x_ && x_ < tabWidths[i]) return i;
                temp = tabWidths[i];
            }
            return -1;
        }

        void selectTab(POINT p,bool hovering_ = false){
            int i = findHoverIndex(p.x);
            if (i >= 0){
                if (hovering_) hover = i;
                else {
                    selectedIndex = i;
                    selectedTab = file_tabs[i];
                }
                return;
            }
        }

        void drawFileTabContainer(HDC hdc,RECT fileTabRect,COLORREF bkcolor){
            HBRUSH bg = CreateSolidBrush(bkcolor);
            tabWidths.clear();
            int x = -scrolled;
            FillRect(hdc,&fileTabRect,bg);
            int temp = 0;

            for (int i = 0;i<file_tabs.size();i++){
                COLORREF color = colorTheme.fileExplorerItemBk;
                std::wstring wtext = string_to_wstring(file_tabs[i]->FileName);
                const wchar_t* text = wtext.c_str();
                int width = calculateFontWidth(text,font);
                temp += width;
                tabWidths.push_back(temp);

                COLORREF bkfileTabcolor = colorTheme.filetabbk;
                COLORREF txtfileTabcolor = colorTheme.filetabtc;
                bool event = false; //true for hover or selected
                
                if (i == selectedIndex){
                    //selected
                    // bkfileTabcolor = RGB(30,147,48);
                    bkfileTabcolor = colorTheme.filetabhoverbk;
                    // txtfileTabcolor = RGB(130,77,48);
                    txtfileTabcolor = colorTheme.filetabhovertc;
                    event = true;
                }
                else if (i == hover){
                    //hover
                    bkfileTabcolor = colorTheme.filetabhoverbk;
                    txtfileTabcolor = colorTheme.filetabhovertc;
                    event = true;
                }
                
                drawFileTab(hdc,file_tabs[i]->FileName,x,0,width,FILE_TAB_HEIGHT,bkfileTabcolor,txtfileTabcolor,event);
                x += width+1;
            }
            if (selectedIndex >= 0){
                std::cout << "hello : " << selectedIndex << "\n";
                if (!selectedIndex) DrawGradientLine(hdc, 0, tabWidths[0]+1, 0, RGB(11, 7, 235), RGB(235, 7, 231));
                else DrawGradientLine(hdc, -scrolled + tabWidths[selectedIndex-1] + selectedIndex, -scrolled + tabWidths[selectedIndex] + selectedIndex + 1, 0, RGB(11, 7, 235), RGB(235, 7, 231));
            }
            DeleteObject(bg);
        }

        void reset(){
            for (int i = 0;i<file_tabs.size();i++){
                delete file_tabs[i];
                file_tabs.clear();
            }
            selectedIndex = -1;
            hover = -1;
            scrolled = 0;
            tabWidths.clear();
        }

};

FileTabContainer fileTabContainer;


LRESULT CALLBACK FileTabContainerProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HBRUSH brush = CreateSolidBrush(colorTheme.filetabContainer);
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT r;
            GetClientRect(hWnd, &r);

            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, r.right, r.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            SelectObject(memDC,fileTabContainer.font);

            fileTabContainer.drawFileTabContainer(memDC,r,colorTheme.filetabContainer);

            BitBlt(hdc, 0, 0, r.right, r.bottom, memDC, 0, 0, SRCCOPY);
           
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            DeleteObject(brush);
            EndPaint(hWnd, &ps);
            return 0;
        }

        case WM_MOUSEHWHEEL:
        {
            RECT r;
            GetClientRect(hWnd,&r);
            int delta = GET_WHEEL_DELTA_WPARAM(wp); // typically 120 or -120
            float scrollAmount = delta * 0.5f; // You can tune this multiplier

            fileTabContainer.scrolled += scrollAmount;

            if (fileTabContainer.scrolled < 0) fileTabContainer.scrolled = 0;
            
            // int m = fileTabContainer.fileTabs.size()*FILE_TAB_WIDTH-r.right;
            int m = fileTabContainer.file_tabs.size()*FILE_TAB_WIDTH-r.right;
            int maxScroll = (0>m)?0:(m);
            if (fileTabContainer.scrolled > maxScroll)
                fileTabContainer.scrolled = maxScroll;

            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);
            fileTabContainer.selectTab(p,true);

            TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            tme.hwndTrack = hWnd;
            // tme.dwHoverTime = HOVER_DEFAULT;
            // tme.dwHoverTime = TME_HOVER; //very fast hovering
            TrackMouseEvent(&tme);

            InvalidateRect(hWnd,NULL,TRUE);
            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);
            if (p.x + fileTabContainer.scrolled > fileTabContainer.tabWidths[fileTabContainer.tabWidths.size()-1]) return 0;
            int index = fileTabContainer.findHoverIndex(p.x);
            if (p.x > fileTabContainer.tabWidths[index]-20){
                //cancel pressed
                fileTabContainer.file_tabs.erase(fileTabContainer.file_tabs.begin() + index);
                FileTab* temp = fileTabContainer.file_tabs[index];
                fileTabContainer.tabWidths.clear();
                
                if (fileTabContainer.selectedIndex > index) fileTabContainer.selectedIndex--;

                if (index == fileTabContainer.selectedIndex){
                    fileTabContainer.selectedIndex--;
                    if (fileTabContainer.selectedIndex < 0) fileTabContainer.selectedIndex = 0;
                    if (fileTabContainer.selectedIndex >= 0 && fileTabContainer.selectedIndex < fileTabContainer.file_tabs.size()){
                        selectedFilePath = fileTabContainer.file_tabs[fileTabContainer.selectedIndex]->FilePath;
                        WriteFileContent(selectedFilePath);
                        SendMessage(hEditor,LOAD_EDITOR_PLUGIN,0,0);
                        InvalidateRect(hEditor,NULL,TRUE);
                    }
                }
                InvalidateRect(hWnd,NULL,TRUE);

                // delete temp;
                return 0;
            }
            fileTabContainer.selectedIndex = index;
            // editContent.clear();
            if (index < fileTabContainer.file_tabs.size() && fileTabContainer.file_tabs[index]){
                selectedFilePath = fileTabContainer.file_tabs[index]->FilePath;

                WriteFileContent(fileTabContainer.file_tabs[index]->FilePath);
                // int id = explorer.getIndex(fileTabContainer.file_tabs[index]->FilePath);
                
                // if (id >= 0 && id != explorer.selected_index){
                    // explorer.selected_index = id;
                    // explorer.selected_project = explorer.visible_item[id];
                    // InvalidateRect(hfileExplorer,NULL,TRUE);
                // }

                SendMessage(hEditor,LOAD_EDITOR_PLUGIN,0,0);
                InvalidateRect(hEditor,NULL,TRUE);
            }
            InvalidateRect(hWnd,NULL,TRUE);
            return 0;
        }

        case WM_MOUSELEAVE:
        {
            fileTabContainer.hover = -1;
            InvalidateRect(hWnd,NULL,TRUE);
            return 0;
        }
    }

    return DefWindowProc(hWnd, msg, wp, lp);

}

HWND createFileTabContainer(HWND hparent){
    RECT r;
    GetClientRect(hparent, &r);
    //adding 14 because r.right because was getting wrong width
    //dont know why
    int w = r.right - FILE_TAB_X + 14;

    hFileTabContainer = CreateWindowEx(0, L"FileTabContainerClass", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
        FILE_TAB_X, FILE_TAB_Y, w, FILE_TAB_HEIGHT, hparent, NULL, NULL, NULL);
    return hFileTabContainer;
}



// LRESULT CALLBACK FileTabContainerProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
//     WNDPROC oldProc = (WNDPROC)GetProp(hWnd, L"OldContainerProc");

//     if (!oldProc)
//         return DefWindowProc(hWnd, msg, wp, lp);

//     switch (msg) {
//         case WM_PAINT: {
//             PAINTSTRUCT ps;
//             HBRUSH brush = CreateSolidBrush(colorTheme.filetabContainer);
//             HDC hdc = BeginPaint(hWnd, &ps);

//             RECT rc;
//             GetClientRect(hWnd, &rc);

//             FillRect(hdc, &rc, brush);

//             DeleteObject(brush);
//             EndPaint(hWnd, &ps);
//             return 0;
//         }
//     }
//     return CallWindowProcW(oldProc, hWnd, msg, wp, lp);


//     // return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA), hWnd, msg, wp, lp);
// }


// LRESULT CALLBACK FileTabProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
//     HFONT font = customFont(FILE_TAB_FONT_SIZE, 8, FILE_TAB_FONT_WEIGHT, L"Segoe UI");

//     FileTab* data = (FileTab*)GetProp(hWnd, L"FileTabPtr");
//     WNDPROC oldProc = (WNDPROC)GetProp(hWnd, L"OldProc");

//     if (!oldProc)
//         return DefWindowProc(hWnd, msg, wp, lp);
    
//     switch (msg) {
        
//         case WM_MOUSEMOVE:
//             if (!data->hover) {
//                 TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
//                 tme.dwFlags = TME_HOVER | TME_LEAVE;
//                 tme.hwndTrack = hWnd;
//                 tme.dwHoverTime = HOVER_DEFAULT;
//                 // tme.dwHoverTime = TME_HOVER; //very fast hovering
//                 TrackMouseEvent(&tme);
//                 data->hover = 1;
//                 InvalidateRect(hWnd,NULL,TRUE);
//             }
//             return 0;
        
//         case WM_MOUSEHOVER:
//             {
//                 HFONT font = customFont(15, 6, FILE_TAB_FONT_WEIGHT, L"Segoe UI");
//                 std::wstring wtext = string_to_wstring(data->FilePath);
//                 const wchar_t* text = wtext.c_str();
//                 int width = calculateFontWidth(text, font);

//                 hoverWindowFilePath = customWindow(text,data->x + FILE_TAB_WIDTH/3,FILE_TAB_Y + FILE_TAB_HEIGHT,width,FILE_TAB_HOVER_WINDOW_HEIGHT,
//                     hmainWindow,colorTheme.hoverWindowColorbk,colorTheme.hoverWindowColortc,0,0,font);

//                 return 0;
//             }

//         case WM_MOUSELEAVE:
//             data->hover = 0;
//             InvalidateRect(hWnd,NULL,TRUE);
//             DestroyWindow(hoverWindowFilePath);

//             return 0;

//         case WM_LBUTTONDOWN: 
//         {
//             if (fileTabs.empty()) return 0;
//             POINT p;
//             GetCursorPos(&p);
//             ScreenToClient(hWnd, &p);

//             RECT r;
//             GetClientRect(hWnd, &r);

//             if (p.x > r.right - FILE_TAB_CLOSE_BUTTON_WIDTH) {
//                 if (data->id == activeFileTabId) activeFileTabId = -1;

//                 else if (activeFileTabId > data->id) activeFileTabId--;

//                 fileTabs.erase(fileTabs.begin() + data->id);

//                 DestroyWindow(hWnd);
//                 DestroyWindow(hoverWindowFilePath);

//                 DestroyWindow(hFileTabContainer);
//                 hFileTabContainer = createFileTabContainer(hmainWindow);
                
//                 return 0;
//             }
//             else {
//                 HWND old;
//                 if (activeFileTabId >= 0){
//                     old = fileTabs[activeFileTabId]->handle;
//                 }
//                 activeFileTabId = data->id;
//                 if (old) InvalidateRect(old,NULL,TRUE);
//                 InvalidateRect(hWnd,NULL,TRUE);
//             }

//             // break;
//             return 0;
//         }

//         case WM_PAINT:
//             {
//             PAINTSTRUCT ps;
//             HBRUSH brush;
//             if (activeFileTabId == data->id) brush = CreateSolidBrush(colorTheme.filetabhoverbk);
//             else brush = CreateSolidBrush(colorTheme.filetabbk);
//             HDC hdc = BeginPaint(hWnd, &ps);

//             RECT rc;
//             GetClientRect(hWnd, &rc);

//             HFONT font = customFont(FILE_TAB_FONT_SIZE, 8, FILE_TAB_FONT_WEIGHT, L"Segoe UI");

//             int k = (rc.bottom - FILE_TAB_HEIGHT) / 2 + 5;

//             FillRect(hdc, &rc, brush);
        

//             SelectObject(hdc, font);
//             SetBkMode(hdc, TRANSPARENT);
//             if (activeFileTabId == data->id) SetTextColor(hdc, colorTheme.filetabhovertc);
//             else SetTextColor(hdc, colorTheme.filetabtc);

//             wchar_t buf[256];
//             GetWindowTextW(hWnd, buf, 256);

//             TextOut(hdc, 8, k, buf, wcslen(buf)); // file name
//             if (data->hover){
//                 // POINT pp;
//                 // GetCursorPos(&pp);
//                 // ScreenToClient(hWnd,&pp);
//                 TextOut(hdc, rc.right - 20, k, L"X", wcslen(L"X"));
//                 // if (pp.y > 0)
//                     // DrawGradientLine(hdc, 0,pp.y, 0, RGB(11, 7, 235), RGB(235, 7, 231));

//             }

//             if (activeFileTabId == data->id){
//                 DrawGradientLine(hdc, 0, rc.right, 0, RGB(11, 7, 235), RGB(235, 7, 231));

//                 // DrawLine(hdc,0,0,rc.right,0,2,colorTheme.hoverwindowhighlightline);
//             }

//             DeleteObject(brush);
//             DeleteObject(font);
//             EndPaint(hWnd, &ps);
//             return 0;
//         }
    

//         case WM_CREATE:
//             SendMessage(hWnd, WM_SETFONT, (WPARAM)font, TRUE);
//             // break;
//             return 0;


//         // case WM_SETFONT:
//             // if (LOWORD(lp)) InvalidateRect(hWnd, NULL, TRUE);
//             // return 0;

//         case WM_DESTROY:
//             RemoveProp(hWnd, L"FileTabPtr");
//             // break;
//             return 0;

//     }
//     return CallWindowProcW(oldProc, hWnd, msg, wp, lp);


//     // return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA), hWnd, msg, wp, lp);
// }

// HWND createFileTab(HWND hWnd, int x, int y, int width, int height, const wchar_t* text, int Id) {
//     HWND tab = CreateWindowEx(0, L"static", text, WS_CHILD | WS_VISIBLE | SS_NOTIFY,
//         x, y, width, height, hWnd, (HMENU)(INT_PTR)Id, NULL, NULL);
    
//     WNDPROC oldProc = (WNDPROC)SetWindowLongPtrW(tab, GWLP_WNDPROC, (LONG_PTR)FileTabProc);
//     SetProp(tab, L"OldProc", (HANDLE)oldProc);
//     // SetWindowLongPtrW(tab, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtrW(tab, GWLP_WNDPROC, (LONG_PTR)FileTabProc));

//     SetProp(tab, L"FileTabPtr", (HANDLE)fileTabs[Id]);

//     return tab;
// }

// HWND createFileTabContainer(HWND hWnd) {
//     RECT r;
//     GetClientRect(hWnd, &r);
//     //adding 14 because r.right because was getting wrong width
//     //dont know why
//     int w = r.right + 14;

//     hFileTabContainer = CreateWindowEx(0, L"static", L"", WS_CHILD | WS_VISIBLE,
//         FILE_TAB_X, FILE_TAB_Y, w, FILE_TAB_HEIGHT, hWnd, NULL, NULL, NULL);
    
//     WNDPROC oldProc = (WNDPROC)SetWindowLongPtrW(hFileTabContainer, GWLP_WNDPROC, (LONG_PTR)FileTabContainerProc);
//     SetProp(hFileTabContainer, L"OldContainerProc", (HANDLE)oldProc);
//     // SetWindowLongPtrW(hFileTabContainer, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtrW(hFileTabContainer, GWLP_WNDPROC, (LONG_PTR)FileTabContainerProc));
//     for (int i = 0; i < fileTabs.size(); i++) {
//         DestroyWindow(fileTabs[i]->handle);
//     }


//     HFONT font = customFont(FILE_TAB_FONT_SIZE, 8, FILE_TAB_FONT_WEIGHT, L"Segoe UI");

//     // int temp = FILE_TAB_X;
//     int temp = 0;

//     for (int i = 0; i < fileTabs.size(); i++) {
//         FileTab* f = fileTabs[i];
//         std::wstring wtext = string_to_wstring(f->FileName);
//         const wchar_t* text = wtext.c_str();
//         int width = calculateFontWidth(text, font);
//         // int width = FILE_TAB_WIDTH;
//         if (temp + width >= w) break;

//         HWND tab = createFileTab(hFileTabContainer, temp, 0, width, FILE_TAB_HEIGHT, text, i);
//         fileTabs[i]->handle = tab;
//         fileTabs[i]->id = i;
//         // fileTabs[i]->x = temp;
//         fileTabs[i]->x = FILE_TAB_X + temp;
//         temp += width + 1;
//     }


//     DeleteObject(font);
//     return hFileTabContainer;
// }

#endif
