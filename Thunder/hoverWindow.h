// // #ifndef HOVERWINDOW_H
// // #define HOVERWINDOW_H

// // #include <windows.h>
// // #include <string>

// // HWND gHoverWindow = NULL;
// // HWND gHoverEdit = NULL;
// // HFONT gHoverFont = NULL;

// // LRESULT CALLBACK HoverWndProc_(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
// //     switch (msg) {
// //         case WM_ACTIVATE:
// //             if (LOWORD(wp) == WA_INACTIVE) {
// //                 DestroyWindow(hwnd);
// //                 gHoverWindow = NULL;
// //                 gHoverEdit = NULL;
// //             }
// //             return 0;
// //     }
// //     return DefWindowProc(hwnd, msg, wp, lp);
// // }

// // void ShowHoverWindow(const std::wstring& text, int x, int y, int width, int height, HWND parent = NULL) {
// //     if (gHoverWindow) return; // Already visible

// //     static bool classRegistered = false;
// //     if (!classRegistered) {
// //         WNDCLASS wc = { 0 };
// //         wc.lpfnWndProc = HoverWndProc_;
// //         wc.hInstance = GetModuleHandle(NULL);
// //         wc.lpszClassName = L"VSHoverWindow";
// //         wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
// //         wc.style = CS_DBLCLKS;
// //         RegisterClass(&wc);
// //         classRegistered = true;
// //     }

// //     gHoverWindow = CreateWindowExW(
// //         0,
// //         L"VSHoverWindow",
// //         NULL,
// //         WS_POPUP | WS_VISIBLE,
// //         x, y, width, height,
// //         parent, NULL, NULL, NULL
// //     );

// //     gHoverEdit = CreateWindowExW(
// //         0, L"EDIT", text.c_str(),
// //         WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL | WS_BORDER,
// //         0, 0, width, height,
// //         gHoverWindow, NULL, GetModuleHandle(NULL), NULL
// //     );

// //     if (!gHoverFont) {
// //         gHoverFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
// //                                  DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
// //                                  CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_MODERN, L"Consolas");
// //     }

// //     SendMessageW(gHoverEdit, WM_SETFONT, (WPARAM)gHoverFont, TRUE);

// //     SendMessageW(gHoverEdit, WM_SETFONT, (WPARAM)gHoverFont, TRUE);

// // }

// // #endif
















// #ifndef HOVERWINDOW_H
// #define HOVERWINDOW_H


// #include <windows.h>
// #include <string>

// class ExtensionWindow {
// public:
//     HWND hwnd = NULL;
//     HFONT font = NULL;
//     std::wstring content;
//     COLORREF backgroundColor = RGB(30, 30, 30);
//     COLORREF textColor = RGB(255, 255, 255);

//     static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
//         ExtensionWindow* self = (ExtensionWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
//         switch (msg) {
//             case WM_CREATE: {
//                 CREATESTRUCT* cs = (CREATESTRUCT*)lp;
//                 self = (ExtensionWindow*)cs->lpCreateParams;
//                 SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)self);
//                 return 0;
//             }
//             case WM_PAINT: {
//                 if (!self) break;
//                 PAINTSTRUCT ps;
//                 HDC hdc = BeginPaint(hwnd, &ps);
//                 SetBkColor(hdc, self->backgroundColor);
//                 SetTextColor(hdc, self->textColor);
//                 SelectObject(hdc, self->font);
//                 RECT r;
//                 GetClientRect(hwnd, &r);
//                 DrawTextW(hdc, self->content.c_str(), -1, &r, DT_LEFT | DT_TOP | DT_WORDBREAK);
//                 EndPaint(hwnd, &ps);
//                 return 0;
//             }
//         }
//         return DefWindowProc(hwnd, msg, wp, lp);
//     }

//     void create(HINSTANCE hInstance, HWND parent, const wchar_t* className = L"ExtensionWindowClass") {
//         WNDCLASS wc = {};
//         wc.lpfnWndProc = WndProc;
//         wc.hInstance = hInstance;
//         wc.lpszClassName = className;
//         wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//         wc.hCursor = LoadCursor(NULL, IDC_ARROW);

//         RegisterClass(&wc);

//         font = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
//                            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
//                            CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");

//         hwnd = CreateWindowExW(WS_EX_TOPMOST, className, L"",
//             WS_POPUP | WS_VISIBLE,
//             CW_USEDEFAULT, CW_USEDEFAULT, 400, 200,
//             parent, NULL, hInstance, this);
//     }

//     void setText(const std::wstring& text) {
//         content = text;
//         InvalidateRect(hwnd, NULL, TRUE);
//     }

//     void show(int x, int y, int w = 400, int h = 200) {
//         SetWindowPos(hwnd, HWND_TOPMOST, x, y, w, h, SWP_SHOWWINDOW);
//     }

//     void hide() {
//         ShowWindow(hwnd, SW_HIDE);
//     }

//     void destroy() {
//         if (hwnd) DestroyWindow(hwnd);
//         if (font) DeleteObject(font);
//     }
// };

// #endif






// TODO : handle scrolling and text copying


#ifndef HOVERWINDOW_H
#define HOVERWINDOW_H


#include <windows.h>
#include <string>

class TextDisplayWindow {
    HWND hWnd = NULL;
    std::wstring text; // the content that needs to be stored
    HFONT font = NULL;

    // background color and text color
    COLORREF bkcolor = RGB(0,0,0),txtcolor = RGB(255,255,255);

    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        TextDisplayWindow* self = reinterpret_cast<TextDisplayWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (self) {
            return self->WndProc(hwnd, msg, wp, lp);
        }
        return DefWindowProc(hwnd, msg, wp, lp);
    }

    LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
            case WM_PAINT: 
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);

                RECT r;
                GetClientRect(hWnd,&r);

                HBRUSH bg = CreateSolidBrush(RGB(100,100,100));
                FillRect(hdc,&r,bg);

                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, RGB(255, 255, 255));

                if (font) SelectObject(hdc, font);

                DrawTextW(hdc, text.c_str(), -1, &r, DT_LEFT | DT_TOP | DT_WORDBREAK);

                EndPaint(hWnd, &ps);
                return 0;
            }

            case WM_KILLFOCUS:
            {
                // will delete the window if clicked outside
                destroy();
                return 0;
            }

            case WM_DESTROY:
            {
                return 0;
            }


        }

        return DefWindowProc(hWnd, msg, wp, lp);
    }

    public:
        int x = 0,y = 0;
        int width = 0,height = 0;
        
        TextDisplayWindow(HWND parent, int x_, int y_, int width_, int height_,COLORREF bkcolor_,COLORREF txtcolor_) {
            // x = x_; y = y_; width = width_; height = height_;
            // bkcolor = bkcolor_;
            // txtcolor = txtcolor_;
            create(parent,x_,y_,width_,height_,bkcolor,txtcolor);
        }

        TextDisplayWindow() {}

        void create(HWND parent,int x_ ,int y_ ,int width_ , int height_,COLORREF bkcolor_,COLORREF txtcolor_) {
            x = x_; y = y_; width = width_; height = height_;
            bkcolor = bkcolor_;
            txtcolor = txtcolor_;

            // hWnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST , L"CustomStaticWindow", L"",WS_POPUP | WS_VISIBLE,
            // hWnd = CreateWindowEx(0, L"CustomStaticWindow", L"",WS_CHILD | WS_VISIBLE,
                // x, y, width, height,parent, NULL, NULL, NULL);


            hWnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE,L"CustomStaticWindow", L"",
                    WS_POPUP,x_, y_, width_, height_,parent, NULL, NULL, NULL);

            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)StaticWndProc);

            font = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
                    CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");
        }

        void setText(const std::wstring& str) {
            text = str;
            InvalidateRect(hWnd, NULL, TRUE); 
            UpdateWindow(hWnd);
        }

        void setFont(HFONT font_){
            // to set new font
            if (font) DeleteObject(font); // delete old font
            
            font = font_;
            InvalidateRect(hWnd,NULL,TRUE);
            UpdateWindow(hWnd);
        }

        void show() {
            // will show window if is hidden
            // ShowWindow(hWnd, SW_SHOW);
            ShowWindow(hWnd, SW_SHOWNOACTIVATE);
        }

        void hide() {
            // will hide the window
            ShowWindow(hWnd, SW_HIDE);
        }

        void destroy() {
            // will destroy the window
            if (hWnd) {
                DestroyWindow(hWnd);
                hWnd = NULL;
                x = -1;
                y = -1;
                width = 0;
                height = 0;
            }
            if (font) {
                DeleteObject(font);
                font = NULL;
            }
        }

        HWND getHandle() const {
            // will return the handle of the window
            return hWnd;
        }
};



// USAGE 

// by default let the handle be hmainWindow
// TextDisplayWindow myTextWindow(hmainWindow,x,y,width,height,background_color,text_color);


// TextDisplayWindow myTextWindow(hmainWindow,200, 200, 300, 150,RGB(0,0,0),RGB(200,200,200));
// myTextWindow.setText(L"Hello, World!\nThis is your custom text window.");
// myTextWindow.show();  

#endif
