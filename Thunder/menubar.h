// //TODO : make this custom container as wm_child and no need to pass the actual screen coordinates
// //TODO : destroy the container window when a menu item is clicked
// //TODO : add hover effect on menu items
// #include <vector>

// #include "window_id.h"
// #include "dimensions.h"
// #include "colors.h"
// #include "drawing.h"
// #include "context_menu.h"

// //some const multiplied with lengh of word
// //to find width of window
// int menu_text_width = 12;

// void FileSubMenu(HWND);
// void EditSubMenu(HWND);


// LRESULT CALLBACK MenuContainerProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
//     switch (msg) {
//         case WM_KILLFOCUS:
//             DestroyWindow(hWnd);
//             return 0;

//         case WM_DESTROY:
//             return 0;
//     }
//     return DefWindowProc(hWnd, msg, wp, lp);
// }


// LRESULT CALLBACK MenuItemProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp){
//     switch (msg){
//         case WM_LBUTTONDOWN:
//             {
//                 int id = GetDlgCtrlID(hWnd);
//                 switch (id){
//                     case FILE_:
//                         FileSubMenu(hmainWindow);
//                         break;
//                     case EDIT:
//                         EditSubMenu(hmainWindow);
//                         break;
//                 }
//                 break;
//             }
        
//         case WM_PAINT:
//             PAINTSTRUCT ps;

//             HBRUSH brush = CreateSolidBrush(colorTheme.hoverWindowColorbk);
//             HDC hdc = BeginPaint(hWnd, &ps);

//             RECT rc;
//             GetClientRect(hWnd,&rc);

//             FillRect(hdc, &rc, brush);

//             SetBkMode(hdc, TRANSPARENT);

//             SetTextColor(hdc, colorTheme.hoverWindowColortc);

//             wchar_t buf[256];
//             GetWindowTextW(hWnd, buf, 256);
//             DrawTextW(hdc, buf, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

//             // DrawLine(hdc,0,0,rc.right,0,4,colorTheme.hoverwindowhighlightline);

//             DeleteObject(brush);  // Clean up
//             EndPaint(hWnd, &ps);
//             return 0;

//     }
//     return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA), hWnd, msg, wp, lp);
// }

// HWND makeItem(HWND hWnd,const wchar_t* text,int x,int y,int width,int height,int ID){
//     HWND h = CreateWindowEx(0,L"static",text,WS_VISIBLE | WS_CHILD | SS_NOTIFY,x,y,width,height,hWnd,(HMENU)(INT_PTR)ID,NULL,NULL);
//     SetWindowLongPtrW(h, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtrW(h, GWLP_WNDPROC, (LONG_PTR)MenuItemProc));
//     return h;
// }

// HWND MenuContainerWindow(HWND hWnd,int x,int y,std::vector<const wchar_t*> texts,std::vector<int> Ids){
//     int size = texts.size(),temp = 0;
//     HWND container = CreateWindowEx(0,L"MenuContainerClass",L"",WS_POPUP | WS_VISIBLE,x,y,SUB_MENU_WIDTH,SUB_MENU_HEIGHT*size,hWnd,NULL,NULL,NULL);

//     // int Hovering = 0;
//     for (int i = 0;i<size;i++){
//         //x = 0 because relatively
//         makeItem(container,texts[i],0,temp,SUB_MENU_WIDTH,SUB_MENU_HEIGHT,Ids[i]);
        
//         temp += SUB_MENU_HEIGHT;
//     }
//     return container;
// }

// int FileMenu(HWND hWnd,int x,int y){
//     int len = 4*menu_text_width; //length of file word * some const
//     hFileM = makeItem(hWnd,L"File",x,y,len,TITLE_BAR_HEIGHT,FILE_);
//     return len;
// }

// int EditMenu(HWND hWnd,int x,int y){
//     int len = 4*menu_text_width; //length of edit word * some const
//     hEditM = makeItem(hWnd,L"Edit",x,y,len,TITLE_BAR_HEIGHT,EDIT);
//     return len;
// }

// void MenuBar(HWND hWnd){
//     int x = FILE_MENU_X,y = FILE_MENU_Y;
//     int len = FileMenu(hWnd,x,y);
//     x += len;
//     len = EditMenu(hWnd,x,y);
//     x += len;
// }

// void FileSubMenu(HWND hWnd){
//     RECT r;
//     GetClientRect(hFileM,&r);
    
//     POINT pt = { r.left, r.bottom };
//     ClientToScreen(hFileM, &pt); // Convert to screen coordinates

//     MenuContainerWindow(hWnd,pt.x,pt.y+1,{L"File",L"Exit"},{OPENFILE,EXIT});
//     // HDC hdc = GetDC(hWnd);
// }

// void EditSubMenu(HWND hWnd){
//     RECT r;
//     GetClientRect(hEditM,&r);

//     POINT pt = { r.left, r.bottom };
//     ClientToScreen(hEditM, &pt); // Convert to screen coordinates
    
//     MapWindowPoints(hEditM,hmainWindow,(LPPOINT)&r,2);
//     MenuContainerWindow(hWnd,pt.x,pt.y+1,{L"Find",L"Replace"},{OPENFILE,EXIT});
// }














//TODO : make this custom container as wm_child and no need to pass the actual screen coordinates
//TODO : destroy the container window when a menu item is clicked
//TODO : add hover effect on menu items
#ifndef MENUBAR_H
#define MENUBAR_H

#include <vector>
#include <gdiplus.h>

#include "window_id.h"
#include "dimensions.h"
#include "colors.h"
#include "drawing.h"
#include "context_menu.h"
#include "font.h"
#include "open_file.h"
#include "file_explorer.h"
#include "string_operations.h"
#include "Buttons.h"
#include "ui_theme.h"
#include "config_handler.h"

// ButtonRect closeButton{200,0,50,30,hmainWindow,L"X",CLOSEBUTTON};
// ButtonRect closeButton(200,0,50,30,L"X",CLOSEBUTTON);
ButtonRect closeButton(CLOSEBUTTON);
// ButtonRect closeButton;


class MenuBar{
    public:
        std::vector<std::wstring> items = {L"File", L"Edit", L"Run"};
        HFONT font = customFont(FILE_TAB_FONT_SIZE, 8, FILE_TAB_FONT_WEIGHT, L"Segoe UI");
        std::vector<int> itemWidths;
        int selectect_index = -1,clicked = 0;
        HWND contextMenu = NULL;

        
        // void init(){
            // closeButton = ButtonRect(100,0,50,30,hmainWindow,L"X",CLOSEBUTTON);
        // }

        int drawItem(HDC hdc,int x,int y,std::wstring text,COLORREF bkcolor,COLORREF txtcolor){
            HBRUSH bg = CreateSolidBrush(bkcolor);
            int width = calculateFontWidth(text.c_str(),font)-10;
            RECT r = {x,y,x+width,y + TITLE_BAR_HEIGHT};
            FillRect(hdc, &r, bg);
            SetBkMode(hdc, TRANSPARENT);  // background drawing
            SetTextColor(hdc,txtcolor);
            TextOut(hdc, x + 15 , 6 ,text.c_str(), wcslen(text.c_str()));
            DeleteObject(bg);
            return width;
        }

        void drawMenuBar(HDC hdc,COLORREF bkcolor,int width){
            HBRUSH bg = CreateSolidBrush(bkcolor);
            RECT r = {FILE_MENU_X,FILE_MENU_Y,FILE_MENU_X + width,FILE_MENU_Y + TITLE_BAR_HEIGHT};
            FillRect(hdc, &r, bg);
            // int temp = 50;
            int temp = FILE_MENU_ITEMS_X;
            itemWidths.clear();
            for (int i = 0;i<items.size();i++){
                COLORREF bcolor = colorTheme.menucolorbk,tcolor = colorTheme.menucolortc;
                if (selectect_index == i){
                    bcolor = colorTheme.menucolorhoverbk;
                    tcolor = colorTheme.menucolorhovertc;
                }
                temp += drawItem(hdc,temp,FILE_MENU_Y,items[i],bcolor,tcolor);
                itemWidths.push_back(temp);
            }
            COLORREF bbcolor = colorTheme.windowControlButtonsbk,btcolor = colorTheme.windowControlButtonstc;
            if (closeButton.isHover){
                bbcolor = colorTheme.hoverButtonColor;
                btcolor = RGB(255,255,255);
            }

            closeButton.draw(hdc,r.right - WINDOW_CLOSE_BUTTON_L,0,WINDOW_CLOSE_BUTTON_L,TITLE_BAR_HEIGHT,L"X",bbcolor,btcolor);
            DeleteObject(bg);
        }

        int findHoverIndex(int x_){
            //given x_ is mouse x
            //will find the index of item the mouse is on 
            // int temp = 50;
            int temp = FILE_MENU_ITEMS_X;
            for (int i = 0;i<itemWidths.size();i++){
                if (temp + i < x_ && x_ < itemWidths[i] + i) return i;
                temp = itemWidths[i];
            }
            return -1;
        }

        std::vector<context_map*> ItemContextMenu(std::vector<std::wstring> subItems,std::vector<int> ids){
            std::vector<context_map*> data;
            for (int i = 0;i<subItems.size();i++){
                context_map* item = new context_map{subItems[i], ids[i]};
                data.push_back(item);
            }
            return data;
        }

        std::vector<context_map*> ContextMenu(int id){
            switch (id){
                case 0:
                    //file
                    return ItemContextMenu({L"Open File",L"Open Folder",L"Save",L"Change Editor Theme"},{OPENFILE,OPENFOLDER,SAVEFILE,CHANGEEDITORTHEME});
                case 1:
                    //edit
                    return ItemContextMenu({L"Find",L"Replace"},{OPENFILE,OPENFILE});
                
                case 2:
                {
                    // run
                    RunProgram program(getCurrentLanguage());
                    program.run(selectedFilePath);
                }
            }
            return {};
        }

        void clearContextMenuItems(std::vector<context_map*> menu){
            for (int i = 0;i< menu.size();i++){
                delete menu[i];
            }
            menu.clear();
        }
};

MenuBar menuBar;


LRESULT CALLBACK MenuContainerProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT r;
            GetClientRect(hWnd, &r);

            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, r.right, r.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
            
            menuBar.drawMenuBar(memDC,colorTheme.menucolorbk,r.right-r.left);

            Gdiplus::Graphics g(memDC);
            Gdiplus::Image icon(L"Images\\thunder.png");
            // g.DrawImage(&icon, Gdiplus::Rect(10, 0, 30, 30));
            g.DrawImage(&icon, Gdiplus::Rect(APP_LOGO_X, APP_LOGO_Y, 30, 30));
            // g.DrawImage(&newfoldericon, r.right - NEW_FOLDER_ICON, 2); //new folder icon
            // g.DrawImage(&icon, r.right - 100, 0); //new folder icon
            
            BitBlt(hdc, 0, 0, r.right, r.bottom, memDC, 0, 0, SRCCOPY);
           
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hWnd, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);
            menuBar.clicked = 1;
            int id = menuBar.findHoverIndex(p.x);
            if (id >= 0 && id < menuBar.items.size()){
                menuBar.selectect_index = id;
            
                std::vector<context_map*> c = menuBar.ContextMenu(id);
            
                if (c.size()){
                    int x = 0;
                    if (id == 0) x = FILE_MENU_ITEMS_X;
                    else x = menuBar.itemWidths[id-1];
                    POINT p = {x,TITLE_BAR_HEIGHT};
                    ClientToScreen(hmainWindow,&p);
                    menuBar.contextMenu  = createContextMenuWindow(p.x,p.y,hWnd,hmainWindow,c);
                    InvalidateRect(hWnd,NULL,TRUE);
                }
            }
            else{
                menuBar.clicked = 0;
                closeButton.click(p,&hmainWindow);
            }

            return 0;
        }

        case WM_MOUSEMOVE:
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);
            int id = menuBar.findHoverIndex(p.x);
            bool c = (id != menuBar.selectect_index);
            menuBar.selectect_index = id;
            
            TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            tme.hwndTrack = hWnd;
            TrackMouseEvent(&tme);
                    
            if (id >= 0 && id < menuBar.items.size()) {
                if (c && menuBar.clicked){
                    std::vector<context_map*> context_menu = menuBar.ContextMenu(id);
            
                    if (context_menu.size()){
                        int x = 0;
                        if (id == 0) x = FILE_MENU_ITEMS_X;
                        else x = menuBar.itemWidths[id-1];
                        POINT p = {x,TITLE_BAR_HEIGHT};
                        ClientToScreen(hmainWindow,&p);
                        menuBar.contextMenu = createContextMenuWindow(p.x,p.y,hWnd,hmainWindow,context_menu);
                        // InvalidateRect(hWnd,NULL,TRUE);
                    }
                }
                InvalidateRect(hWnd,NULL,TRUE);
            }
            if (closeButton.hover(p)){
                closeButton.isHover = 1;
                InvalidateRect(hWnd,NULL,TRUE);
            }
            return 0;
        }

        case WM_COMMAND:
        {
            switch (wp){
                case OPENFILE:
                {
                    return 0;
                }

                case OPENFOLDER:
                {
                    // to open a project
                    std::wstring f_path = openFolder(hmainWindow);
                    editContent.clear();
                    explorer.reset();
                    fileTabContainer.reset();
                    std::string str_ = wstring_string(f_path);
                    // explorer.open_project(wstring_string(f_path));
                    explorer.open_project(str_);
                    ConfigHandler::appendInRecentFolders(str_);
                    InvalidateRect(hfileExplorer,NULL,TRUE);
                    InvalidateRect(hFileTabContainer,NULL,TRUE);
                    InvalidateRect(hEditor,NULL,TRUE);

                    return 0;
                }

                case SAVEFILE:
                {
                    //to save file
                    std::vector<std::wstring> content = editContent;
                    save_file(selectedFilePath,content);
                    return 0;
                }

                case CHANGEEDITORTHEME:
                {
                    // to change editor theme
                    std::vector<context_map*> menu;

                    for (int i = 0;i<availableEditorThemes.size();i++){
                        context_map* item = new context_map{availableEditorThemes[i]->name, CHANGEEDITORTHEME};
                        menu.push_back(item);
                    }
                    createContextMenuWindow(CW_USEDEFAULT,CW_USEDEFAULT,hmainWindow,hmainWindow,menu);
                    return 0;
                }
            }
            return 0;
        }

        case WM_MOUSELEAVE:
        {
            menuBar.selectect_index = -1;
            closeButton.isHover = 0;
            InvalidateRect(hWnd,NULL,TRUE);
            menuBar.clicked = 0;
            return 0;
        }

        case WM_DESTROY:
            return 0;
    }
    return DefWindowProc(hWnd, msg, wp, lp);
}


HWND createMenuBar(HWND hparent,int width){
    hmenuBar = CreateWindowEx(0, L"MenuContainerClass", L"", WS_CHILD | WS_VISIBLE ,
        FILE_MENU_X, FILE_MENU_Y, width, TITLE_BAR_HEIGHT, hparent, NULL, NULL, NULL);
    return hmenuBar;
}

#endif

// LRESULT CALLBACK MenuContainerProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
//     switch (msg) {
//         case WM_KILLFOCUS:
//             DestroyWindow(hWnd);
//             return 0;

//         case WM_DESTROY:
//             return 0;
//     }
//     return DefWindowProc(hWnd, msg, wp, lp);
// }


// LRESULT CALLBACK MenuItemProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp){
//     switch (msg){
//         case WM_LBUTTONDOWN:
//             {
//                 int id = GetDlgCtrlID(hWnd);
//                 switch (id){
//                     case FILE_:
//                         FileSubMenu(hmainWindow);
//                         break;
//                     case EDIT:
//                         EditSubMenu(hmainWindow);
//                         break;
//                 }
//                 break;
//             }
        
//         case WM_PAINT:
//             PAINTSTRUCT ps;

//             HBRUSH brush = CreateSolidBrush(colorTheme.hoverWindowColorbk);
//             HDC hdc = BeginPaint(hWnd, &ps);

//             RECT rc;
//             GetClientRect(hWnd,&rc);

//             FillRect(hdc, &rc, brush);

//             SetBkMode(hdc, TRANSPARENT);

//             SetTextColor(hdc, colorTheme.hoverWindowColortc);

//             wchar_t buf[256];
//             GetWindowTextW(hWnd, buf, 256);
//             DrawTextW(hdc, buf, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

//             // DrawLine(hdc,0,0,rc.right,0,4,colorTheme.hoverwindowhighlightline);

//             DeleteObject(brush);  // Clean up
//             EndPaint(hWnd, &ps);
//             return 0;

//     }
//     return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA), hWnd, msg, wp, lp);
// }

// HWND makeItem(HWND hWnd,const wchar_t* text,int x,int y,int width,int height,int ID){
//     HWND h = CreateWindowEx(0,L"static",text,WS_VISIBLE | WS_CHILD | SS_NOTIFY,x,y,width,height,hWnd,(HMENU)(INT_PTR)ID,NULL,NULL);
//     SetWindowLongPtrW(h, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtrW(h, GWLP_WNDPROC, (LONG_PTR)MenuItemProc));
//     return h;
// }

// HWND MenuContainerWindow(HWND hWnd,int x,int y,std::vector<const wchar_t*> texts,std::vector<int> Ids){
//     int size = texts.size(),temp = 0;
//     HWND container = CreateWindowEx(0,L"MenuContainerClass",L"",WS_POPUP | WS_VISIBLE,x,y,SUB_MENU_WIDTH,SUB_MENU_HEIGHT*size,hWnd,NULL,NULL,NULL);

//     // int Hovering = 0;
//     for (int i = 0;i<size;i++){
//         //x = 0 because relatively
//         makeItem(container,texts[i],0,temp,SUB_MENU_WIDTH,SUB_MENU_HEIGHT,Ids[i]);
        
//         temp += SUB_MENU_HEIGHT;
//     }
//     return container;
// }

// int FileMenu(HWND hWnd,int x,int y){
//     int len = 4*menu_text_width; //length of file word * some const
//     hFileM = makeItem(hWnd,L"File",x,y,len,TITLE_BAR_HEIGHT,FILE_);
//     return len;
// }

// int EditMenu(HWND hWnd,int x,int y){
//     int len = 4*menu_text_width; //length of edit word * some const
//     hEditM = makeItem(hWnd,L"Edit",x,y,len,TITLE_BAR_HEIGHT,EDIT);
//     return len;
// }

// void MenuBar(HWND hWnd){
//     int x = FILE_MENU_X,y = FILE_MENU_Y;
//     int len = FileMenu(hWnd,x,y);
//     x += len;
//     len = EditMenu(hWnd,x,y);
//     x += len;
// }

// void FileSubMenu(HWND hWnd){
//     RECT r;
//     GetClientRect(hFileM,&r);
    
//     POINT pt = { r.left, r.bottom };
//     ClientToScreen(hFileM, &pt); // Convert to screen coordinates

//     MenuContainerWindow(hWnd,pt.x,pt.y+1,{L"File",L"Exit"},{OPENFILE,EXIT});
//     // HDC hdc = GetDC(hWnd);
// }

// void EditSubMenu(HWND hWnd){
//     RECT r;
//     GetClientRect(hEditM,&r);

//     POINT pt = { r.left, r.bottom };
//     ClientToScreen(hEditM, &pt); // Convert to screen coordinates
    
//     MapWindowPoints(hEditM,hmainWindow,(LPPOINT)&r,2);
//     MenuContainerWindow(hWnd,pt.x,pt.y+1,{L"Find",L"Replace"},{OPENFILE,EXIT});
// }
