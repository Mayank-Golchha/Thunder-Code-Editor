//this is the window that gets open when you click right mouse button
//will communicate the parent window by wm_command 
//the main message will be in wparam

#ifndef CONTEXT_MENU_H
#define CONTEXT_MENU_H

#include <windows.h>
#include <string>
#include <vector>

#include "drawing.h"
#include "dimensions.h"
#include "colors.h"

struct context_map{
    std::wstring name;
    int message_id; //will be send to parent window when name is clicked
};


LRESULT CALLBACK ContextMenuWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    std::vector<context_map*>* data = (std::vector<context_map*>*)GetProp(hWnd, L"context_map");
    int hover_index = (int)(INT_PTR)GetProp(hWnd, L"hover_index");

    switch (msg){
        case WM_MOUSEMOVE:
        {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);
            if (hover_index == p.y/CONTEXT_MENU_LINE_HEIGHT) return 0;
            hover_index = p.y/CONTEXT_MENU_LINE_HEIGHT;
            SetProp(hWnd, L"hover_index", (HANDLE)(INT_PTR)(hover_index));
            InvalidateRect(hWnd,NULL,TRUE);
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT r;
            GetClientRect(hWnd, &r);

            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);
            HBRUSH bg = CreateSolidBrush(colorTheme.contextMenuBk);
            
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, r.right, r.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            FillRect(memDC,&r,bg);

            SetTextColor(memDC,colorTheme.contextMenuTc);

            SetBkMode(memDC,TRANSPARENT);

            
            HFONT font = CreateFont(-CONTEXT_MENU_FONT_HEIGHT, 0, 0, 0,
                500,FALSE, FALSE, FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH | FF_MODERN,L"Consolas"             // Font name
            );

            SelectObject(memDC,font);

            for (int i = 0;i<data->size();i++){
                if (i == hover_index){
                    //hover
                    SetTextColor(memDC,colorTheme.contextMenuHighLightTc);
                    RECT rp = {0,i*CONTEXT_MENU_LINE_HEIGHT,r.right,(i+1)*CONTEXT_MENU_LINE_HEIGHT};
                    bg = CreateSolidBrush(colorTheme.contextMenuHighLightBk);
                    FillRect(memDC,&rp,bg);   
                    SetBkMode(memDC,TRANSPARENT);
                    TextOut(memDC,CONTEXT_MENU_X_OFFSET,i*CONTEXT_MENU_LINE_HEIGHT + 5,(*data)[i]->name.c_str(),wcslen((*data)[i]->name.c_str()));
                    SetTextColor(memDC,colorTheme.contextMenuTc);
                }
                else TextOut(memDC,CONTEXT_MENU_X_OFFSET,i*CONTEXT_MENU_LINE_HEIGHT + 5,(*data)[i]->name.c_str(),wcslen((*data)[i]->name.c_str()));
            } 
            DrawGradientLine(memDC, 0, r.right, r.bottom-2, RGB(11, 7, 235), RGB(235, 7, 231));

            BitBlt(hdc, 0, 0, r.right, r.bottom, memDC, 0, 0, SRCCOPY);
           
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);
            
            DeleteObject(bg);
            EndPaint(hWnd, &ps);
            return 0;
        }
        case WM_KILLFOCUS:
        {
            DestroyWindow(hWnd);
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            // POINT pt = { LOWORD(lp), HIWORD(lp)};
            RECT r;
            GetClientRect(hWnd, &r);
            // if (!PtInRect(&r, pt)) {
                // DestroyWindow(hWnd);  // clicked outside
            // }
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);

            // RECT r;
            // GetClientRect(hWnd,&r);

            int index = p.y/CONTEXT_MENU_LINE_HEIGHT;
            // HWND parent = GetParent(hWnd);
            HWND parent = (HWND)GetProp(hWnd,L"parent");
            SendMessage(parent,WM_COMMAND,(WPARAM)(INT_PTR)(*data)[index]->message_id,index);
            DestroyWindow(hWnd);

            return 0;
        }

        // case WM_CAPTURECHANGED:
            // DestroyWindow(hWnd);  // focus/click moved elsewhere
            // return 0;

        case WM_DESTROY:
        {
            for (int i = 0;i<data->size();i++) delete (*data)[i];
            RemoveProp(hWnd, L"context_map");    
            RemoveProp(hWnd, L"hover_index");    
            RemoveProp(hWnd, L"parent");    
            return 0;
        }


    }
    return DefWindowProc(hWnd, msg, wp, lp);
}

HWND createContextMenuWindow(int x,int y,HWND parent,HWND drawing_parent,std::vector<context_map*>& data){
    //drawing parent is the main window in which it will be draw and parent is the one who it will respond
    if (!data.size()) return NULL;
    auto* data_ = new std::vector<context_map*>(data);
    HWND w = CreateWindowEx(0,L"ContextMenuWindowClass", L"",
        // WS_VISIBLE | WS_CHILD , x, y, CONTEXT_MENU_WIDTH,CONTEXT_MENU_LINE_HEIGHT*data.size(),
        WS_POPUP | WS_VISIBLE | WS_BORDER, x, y, CONTEXT_MENU_WIDTH,CONTEXT_MENU_LINE_HEIGHT*data.size(),
        drawing_parent, NULL, NULL, NULL);

    SetProp(w, L"context_map", reinterpret_cast<HANDLE>(data_));
    SetProp(w, L"parent", parent);
    SetProp(w, L"hover_index", (HANDLE)(INT_PTR)(-1));

    // SetProp(w,L"context_map",(HANDLE)data);
    return w;
}


#endif