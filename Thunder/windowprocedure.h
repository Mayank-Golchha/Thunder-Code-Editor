// TODO : handle resizing
// TODO : read ui_config file and read app theme

#include "window_handles.h"
// #include "api/API_Mananger.h"
#include "editor.h"
#include "Buttons.h"
#include "dimensions.h"
#include "colors.h"
#include "window_id.h"
#include "menubar.h"
#include "FileTab.h"
#include "file_explorer.h"
#include "context_menu.h"

#include <iostream>
#include <dwmapi.h>

// int hoverMenuBarItem = 0;
// Button_ bt(L"Heloo",400,0,100,30,RED,WHITE,GREEN,BLACK,YELLOW,WHITE);

bool HoverButton(HWND hWnd,POINT* cursor_pos){
    POINT pt = *cursor_pos; // copy so we don't modify original
    RECT r;
    GetWindowRect(hWnd, &r);
    return PtInRect(&r, pt);
}

void CloseButtonMessage(HWND hWnd){
    int result = MessageBoxEx(hWnd,L"Want to Exit ?",L"Confirm Exit",MB_YESNO | MB_ICONQUESTION,0);
    if (result == IDYES){
        //TODO : save content
        // PostQuitMessage(0);
        DestroyWindow(hWnd);
    }
}

void DrawTitleBar(HDC hdc, RECT rc) {
    HBRUSH brush = CreateSolidBrush(colorTheme.titlebar);
    RECT title = { rc.left, rc.top, rc.right, rc.top + TITLE_BAR_HEIGHT };
    FillRect(hdc, &title, brush);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255,255,255));
    
    //
    // RECT closeButton = {rc.right-40,rc.top,rc.right,rc.top + TITLE_BAR_HEIGHT};
    // brush = CreateSolidBrush(RGB(50,50,20));
    // FillRect(hdc,&closeButton,brush);
    // SetTextColor(hdc, colorTheme.filetabtc);
    // TextOut(hdc, rc.right - 25, 5, L"X", wcslen(L"X"));
    //
    // bt.draw(hdc,0,0,16);

    title.left += 200;
    title.top += 5;
    DrawText(hdc, L"Text Editor", -1, &title, DT_SINGLELINE | DT_LEFT);
    DeleteObject(brush);
}

void create(HWND hWnd){
    RECT r;

    GetClientRect(hWnd,&r);

    // hcloseWindowButton = Button(L"X",r.right - WINDOW_CLOSE_BUTTON_L,0,WINDOW_CLOSE_BUTTON_L,TITLE_BAR_HEIGHT,hWnd,CLOSEBUTTON,colorTheme.windowControlButtonsbk,
                // colorTheme.windowControlButtonstc,colorTheme.hoverButtonColor);
    // MenuBar(hWnd);    
    createMenuBar(hWnd,r.right-r.left+16);
    // createMenuBar(hWnd,r.right-r.left+16-100);

    // fileTabs.push_back(a);
    // fileTabs.push_back(b);
    // fileTabs.push_back(c);
    // fileTabs.push_back(d);
    // fileTabs.push_back(e);
    // fileTabs.push_back(f);
    // fileTabs.push_back(g);
    // fileTabs.push_back(hi);
    // fileTabs.push_back(i);
    createFileTabContainer(hWnd);

    createEditor(hWnd,EDITOR_X,EDITOR_Y,r.right-EDITOR_X + 14,r.bottom-EDITOR_Y + 14);

    hfileExplorer = createFileExplorer(hWnd,FILE_EXPLORER_X,FILE_EXPLORER_Y,FILE_EXPLORER_WIDTH,r.bottom-FILE_EXPLORER_Y+14);

    // createFileTab(hWnd,FILE_TAB_X,FILE_TAB_Y,FILE_TAB_HEIGHT,L"Hello World",FILETAB,"","");
}

void resize(HWND hWnd){
    RECT r;
    GetClientRect(hWnd,&r);
    // MoveWindow(hcloseWindowButton,r.right - WINDOW_CLOSE_BUTTON_L,0,WINDOW_CLOSE_BUTTON_L,TITLE_BAR_HEIGHT,FALSE);
    InvalidateRect(hWnd,NULL,TRUE);
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
    switch (msg){
        case WM_PAINT: 
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                
                RECT rc;
                GetClientRect(hWnd, &rc);

                HBRUSH bg = CreateSolidBrush(colorTheme.mainWindow);

                FillRect(hdc, &rc, bg);

                DeleteObject(bg);

                DrawTitleBar(hdc, rc);

                EndPaint(hWnd, &ps);
                return 0;
            }

        // case WM_SIZE:
        //     {
        //         RECT r;
        //         GetClientRect(hWnd, &r);
        //         static int lastX = -1;
        //         int newX = r.right - WINDOW_CLOSE_BUTTON_L;

        //         if (newX != lastX) {
        //             MoveWindow(hcloseWindowButton,r.right - WINDOW_CLOSE_BUTTON_L,0,WINDOW_CLOSE_BUTTON_L,TITLE_BAR_HEIGHT,TRUE);
        //             lastX = newX;
        //         }
        //         return 0;
        //     }
        
        case WM_NCCALCSIZE:
            if (wp) return 0;
            break;

        // case WM_NCHITTEST: {
        //     // Allow dragging the window
        //     POINT pt = {LOWORD(lp), HIWORD(lp) };
        //     ScreenToClient(hWnd, &pt);
        //     return HTCAPTION; // Allow click-drag anywhere
        // }

        case WM_NCHITTEST: {
            POINT pt = { LOWORD(lp), HIWORD(lp) };
            ScreenToClient(hWnd, &pt);
            RECT rc;
            GetClientRect(hWnd, &rc);
            const int border = 8;

            if (pt.y < border) return HTTOP;
            if (pt.y >= rc.bottom - border) return HTBOTTOM;
            if (pt.x < border) return HTLEFT;
            if (pt.x >= rc.right - border) return HTRIGHT;
             // draggable title bar area
            if (pt.y < TITLE_BAR_HEIGHT && pt.x < 300) return HTCAPTION;
            return HTCLIENT;
        }

        case WM_CREATE:
        {            
            MARGINS margins = { 0, 0, 0, 0 };

            // MARGINS margins = { -1 }; // Negative margins extend the full window
            DwmExtendFrameIntoClientArea(hWnd, &margins);
            create(hWnd);
            return 0;
        }

        case WM_COMMAND:
            switch (wp){
                case CLOSEBUTTON:
                    CloseButtonMessage(hWnd);
                    break;

                case CHANGEEDITORTHEME:
                {
                    //to change theme
                    int id = lp;
                    colorTheme = loadTheme(availableEditorThemes[id]->path,default_theme);
                    RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
                    return 0;     
                }
            }
            return 0;

        // case WM_CTLCOLORSTATIC:
        // {
            // HWND hwndStatic = (HWND)lp;
            // if (hwndStatic == hhover) {
                // std::cout << "match";
                // HDC hdc = (HDC)wp;
                // SetTextColor(hdc, RGB(220, 220, 220));
                // SetBkColor(hdc, RGB(30, 30, 30));
                // return (INT_PTR)CreateSolidBrush(RGB(30, 30, 30));
            // }
            // return 0;
        // }


        case WM_ERASEBKGND:
            //An application should return nonzero if it erases the background; otherwise, it should return zero.
            return 1;

        case WM_NCPAINT:
            //The WM_NCPAINT message is sent to a window when its frame must be painted.
            //An application returns zero if it processes this message.
            return 0;

        case WM_NCACTIVATE:
            return TRUE;

        case WM_LBUTTONDOWN:
            SetCapture(hWnd);
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);

            // if (bt.isHover(p.x,p.y)) {
                // bt.isClicked = 1;
                // InvalidateRect(hWnd,NULL,TRUE);
            // }
            return 0;

        case WM_LBUTTONUP:
            ReleaseCapture();
            // if (hoverMenuBarItem)
            // InvalidateRect(hWnd,NULL,TRUE);
            return 0;


        case WM_DESTROY:
            PostQuitMessage(0);
            // writeOpenFileTabsInFile(fileTabContainer.file_tabs);
            return 0;
        

    }
    return DefWindowProc(hWnd, msg, wp, lp);
}