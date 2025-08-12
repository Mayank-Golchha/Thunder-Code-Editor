//this windows is for taking input from user and return it to the parent window
//will send via INPUT_WINDOW_MESSAGE so parent window needs to recieve this message
//there is some message that it will send via lp to find what kind of message it is

//TODO : this window is currently reporting to file_explorer only

#ifndef INPUTWINDOW_H
#define INPUTWINDOW_H

// #ifndef INPUT_WINDOW_MESSAGE
// #define INPUT_WINDOW_MESSAGE WM_USER + 10
// #endif

#include <windows.h>
#include <string>
#include <dwmapi.h>

#include "dimensions.h"
#include "colors.h"
#include "drawing.h"
#include "string_operations.h"
#include "window_id.h"

HFONT inputWindowFont = CreateFont(-INPUT_WINDOW_FONT_SIZE, 0, 0, 0,
    500,FALSE, FALSE, FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH | FF_MODERN,L"Consolas"             // Font name
);

int inputWindowFontSize = 0;

LRESULT CALLBACK InputWindowProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp){
    std::wstring* data = (std::wstring*)GetProp(hWnd, L"InputWindowBuffer");
    int pos = (int)(INT_PTR)GetProp(hWnd, L"InputWindowCursorPos");

    switch (msg){
        case WM_CHAR:
        {
            // pos >= 100 || pos < 0 -> if xpos greater than maxlen dont do anything
            if (wp < VK_SPACE || pos >= 100 || pos < 0) return 0;
            data->insert(data->begin()+pos,wp);
            InvalidateRect(hWnd,NULL,TRUE);
            SetProp(hWnd, L"InputWindowCursorPos", (HANDLE)(INT_PTR)(pos+1));
            SetCaretPos((pos+1)*inputWindowFontSize,INPUT_WINDOW_WRITING_PART_Y);
            return 0;
        }

        case WM_KEYDOWN:
        {
            switch (wp){
                case VK_RIGHT:
                {
                    if (pos < 100){
                        SetCaretPos(INPUT_WINDOW_WRITING_PART_X + (pos+1)*inputWindowFontSize,INPUT_WINDOW_WRITING_PART_Y);
                        SetProp(hWnd, L"InputWindowCursorPos", (HANDLE)(INT_PTR)(pos+1));
                    }
                    break;
                }
                case VK_LEFT:
                {
                    if (pos > 1){
                        SetCaretPos(INPUT_WINDOW_WRITING_PART_X + (pos-1)*inputWindowFontSize,INPUT_WINDOW_WRITING_PART_Y);
                        SetProp(hWnd, L"InputWindowCursorPos", (HANDLE)(INT_PTR)(pos-1));
                    }
                    break;
                }
                case VK_RETURN:
                {
                    HWND parent = GetParent(hWnd);
                    int message = (int)(INT_PTR)GetProp(hWnd, L"InputWindowMessageResponse");
                    std::string d_str = wstring_string(*data);
                    std::cout << "str : " << d_str;
                    SendMessage(hfileExplorer,INPUT_WINDOW_MESSAGE,(WPARAM)data->c_str(),message);
                    std::cout << "\n m " << message;

                    // SendMessage(parent,INPUT_WINDOW_MESSAGE,(WPARAM)d_str.c_str(),message);
                    // SendMessage(hfileExplorer,INPUT_WINDOW_MESSAGE,(WPARAM)d_str.c_str(),(LPARAM)(INT_PTR)message);

                    // LRESULT R = SendMessage(hfileExplorer,INPUT_WINDOW_MESSAGE,0,0);
                    // std::cout << "\n" << R;

                    DestroyWindow(hWnd);
                    break;
                } 

                case VK_BACK:
                {
                    if (data->size() > 0){
                        data->erase(data->begin()+pos-1);
                        SetCaretPos(INPUT_WINDOW_WRITING_PART_X + (pos-1)*inputWindowFontSize,INPUT_WINDOW_WRITING_PART_Y);
                        SetProp(hWnd, L"InputWindowCursorPos", (HANDLE)(INT_PTR)(pos-1));
                        InvalidateRect(hWnd,NULL,TRUE);
                    }
                    return 0;

                }
            }
            return 0;
        }
      
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HBRUSH brush = CreateSolidBrush(colorTheme.inputWindowBk);
            
            HDC hdc = BeginPaint(hWnd, &ps);
            

            RECT r;
            GetClientRect(hWnd,&r);

            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, ps.rcPaint.right, ps.rcPaint.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            FillRect(memDC,&r,brush);
            SetBkMode(memDC,TRANSPARENT);
            SelectObject(memDC, inputWindowFont);

            SetTextColor(memDC,colorTheme.inputWindowTc);
            TextOutW(memDC, INPUT_WINDOW_WRITING_PART_X, INPUT_WINDOW_WRITING_PART_Y, data->c_str(), static_cast<int>(data->length()));

            DrawGradientLine(memDC, 0, r.right, r.bottom-2, RGB(11, 7, 235), RGB(235, 7, 231));

            BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, memDC, 0, 0, SRCCOPY);
            SelectObject(memDC, oldBitmap);

            DeleteDC(memDC);

            // FillRect(hdc, &rc, brush);

            DeleteObject(brush);  
            EndPaint(hWnd, &ps);
            return 0;
        }

        case WM_KILLFOCUS:
        {
            DestroyWindow(hWnd);
            return 0;
        }

        case WM_SETFOCUS:
        {
            CreateCaret(hWnd, (HBITMAP) NULL, 1, 18); 
            SetCaretPos(INPUT_WINDOW_WRITING_PART_X, INPUT_WINDOW_WRITING_PART_Y); 
            ShowCaret(hWnd); 
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            SetFocus(hWnd);
            // SendMessage(hEditor,WM_KILLFOCUS,0,0);
            return 0;
        }

        case WM_CREATE:
        {
            HDC hdc = GetDC(hWnd);
            SIZE sz;
            SelectObject(hdc,inputWindowFont);
            GetTextExtentPoint32(hdc, L"a", 1, &sz);
            inputWindowFontSize = sz.cx;
            ReleaseDC(hWnd,hdc);

            return 0;
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_DESTROY:
        {
            // delete data;
            RemoveProp(hWnd,L"InputWindowBuffer");
            RemoveProp(hWnd,L"InputWindowCursorPos");
            RemoveProp(hWnd,L"InputWindowMessageResponse");

            return 0;
        }

        case WM_GETDLGCODE:
            // Sent to the window procedure associated with a control
            // By default, the system handles all keyboard input to the control;
            // the system interprets certain types of keyboard input as dialog box navigation keys.
            // To override this default behavior, the control can respond to the 
            // WM_GETDLGCODE message to indicate the types of input it wants to process itself.
            return DLGC_WANTCHARS | DLGC_WANTARROWS;

        return 0;
    }

    return DefWindowProc(hWnd, msg, wp, lp);

    // return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA), hWnd, msg, wp, lp);
}


HWND createInputWindow(int x,int y,int width,int height,HWND parent,int message){
    //message is the thing that will be send via lparam when enter is pressed
    HWND w = CreateWindowEx(0,L"InputWindowClass",L"",WS_VISIBLE | WS_CHILD | WS_BORDER ,x,y,width,height,parent,NULL,NULL,NULL);
    // HWND w = CreateWindowEx(0,L"static",L"",WS_VISIBLE | WS_POPUP | WS_BORDER,0,0,width,height,NULL,NULL,NULL,NULL);
    // HWND w = CreateWindowEx(0,L"static",L"",WS_POPUP,0,0,width,height,NULL,NULL,NULL,NULL);

    // SetWindowLongPtrW(w, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtrW(w, GWLP_WNDPROC, (LONG_PTR)InputWindowProc));
    std::wstring *data = new std::wstring;
    int x_coor = 0;
    SetProp(w, L"InputWindowBuffer", (HANDLE)data);
    SetProp(w, L"InputWindowCursorPos", (HANDLE)(INT_PTR)x_coor);
    SetProp(w, L"InputWindowMessageResponse", (HANDLE)(INT_PTR)message);

    return w;
}

#endif
