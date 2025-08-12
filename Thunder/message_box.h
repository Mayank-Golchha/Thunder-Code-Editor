#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include <windows.h>

#include "Buttons.h"


class MessageWindow{
    HWND hWnd;
    static constexpr LPCWSTR s_class = L"MessageWindowClass";
    COLORREF bkcolor,txtcolor;

    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
        MessageWindow* pThis = nullptr;
        if (msg == WM_NCCREATE) {
            pThis = static_cast<MessageWindow*>(reinterpret_cast<CREATESTRUCT*>(l)->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
            pThis->hWnd = hwnd;
        } 
        else pThis = reinterpret_cast<MessageWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        return pThis? pThis->WndProc(msg, w, l) : DefWindowProc(hwnd, msg, w, l);
    }

    LRESULT WndProc(UINT msg, WPARAM w, LPARAM l) {
        switch (msg) {
            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);

                RECT r;
                GetClientRect(hWnd, &r);

                EndPaint(hWnd, &ps);
                return 0;
            }

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

        }
        return DefWindowProc(hWnd, msg, w, l);
    }

    public:
        std::vector<ButtonRect> buttons;
        MessageWindow(HWND hparent,int x,int y,int width,int height,COLORREF bk_color,COLORREF text_color){
            bkcolor = bk_color;
            txtcolor = text_color;
            WNDCLASS wc = {};
            wc.lpfnWndProc   = StaticWndProc;
            wc.hInstance = GetModuleHandle(NULL);
            wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wc.lpszClassName = s_class;
            RegisterClass(&wc);
            hWnd = CreateWindowEx(0, s_class, L"",WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE, x, y,
                width, height, hparent, NULL, NULL, NULL);
        }

        // void addButton(int x,int y,int width,int height,std::wstring text,COLORREF button_bk_color,COLORREF button_txt_color){
            // ButtonRect button()
        // }

};




#endif