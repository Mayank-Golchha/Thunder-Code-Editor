#ifndef BUTTONS_H
#define BUTTONS_H

#include "colors.h"
#include "hoverWindow.h"

// #include <windows.h>

int hovering = 0;

struct ButtonDetails{
    COLORREF bkcolor;
    COLORREF txtcolor;
    COLORREF hovercolor;
    int hovering;
    int x;
    int y;
};

// WNDPROC oldProc;


LRESULT CALLBACK ButtonProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp){
    ButtonDetails* data = (ButtonDetails*)GetProp(hWnd, L"ButtonDetails");
    WNDPROC oldProc = (WNDPROC)GetProp(hWnd, L"OldProc");

    if (!data || !oldProc)
        return DefWindowProc(hWnd, msg, wp, lp);

    switch (msg){
        case WM_MOUSEMOVE:
            {
                if (!data->hovering){

                    TRACKMOUSEEVENT tme = { sizeof(tme) };
                    tme.dwFlags = TME_LEAVE;
                    tme.hwndTrack = hWnd;
                    TrackMouseEvent(&tme);
                    data->hovering = 1;
                    InvalidateRect(hWnd,NULL,TRUE);

                    // return 0;
                }

            }
            return 0;
            // break;
        
        case WM_NCHITTEST:
            return HTCLIENT;
        
        case WM_MOUSELEAVE:
            // hovering = 0;
            data->hovering = 0;
            InvalidateRect(hWnd,NULL,TRUE);
            return 0;
        
        case BN_CLICKED:
            InvalidateRect(hWnd,NULL,TRUE);
            return 0;

        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HBRUSH brush;
                
                RECT rc;
                GetClientRect(hWnd,&rc);
                
                HDC hdc = BeginPaint(hWnd, &ps);
                
                if (data->hovering) brush = CreateSolidBrush(data->hovercolor);
                else brush = CreateSolidBrush(data->bkcolor);
                
                FillRect(hdc, &rc, brush);
                
                wchar_t text[256]; //extracting the text which was passed in the createwindowex function
                GetWindowText(hWnd, text, 256);

                SetBkMode(hdc, TRANSPARENT);
                // SetTextColor(hdc, data->txtcolor);
                SetTextColor(hdc, RGB(255,255,255));
                DrawText(hdc, text, -1,&rc, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER);
                
                DeleteObject(brush);  // Clean up
                EndPaint(hWnd, &ps);

                return 0;
            }
        
        case WM_SETFOCUS:
            InvalidateRect(hWnd,NULL,TRUE);
            return 0;
        
        case WM_KILLFOCUS:
            InvalidateRect(hWnd,NULL,TRUE);
            return 0;
        
        default:
            return CallWindowProcW(oldProc, hWnd, msg, wp, lp);

            // return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA), hWnd, msg, wp, lp);
        
    }
}

HWND Button(const wchar_t* text,int x,int y,int width,int height,HWND parent,int ID,COLORREF bkcolor,COLORREF txtcolor,COLORREF hovercolor){
    HWND hButton = CreateWindowEx(
    0,
    L"button",   // Button class
    text,        // No text
    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, // Styles
    x, y, width, height,  // Position and size
    parent,    // Parent window handle
    (HMENU)(INT_PTR)ID,  // Button ID
    NULL,      // Instance handle
    NULL       // Creation data
    );
    // SetWindowLongPtrW(hButton, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtrW(hButton, GWLP_WNDPROC, (LONG_PTR)ButtonProc));
    WNDPROC oldProc = (WNDPROC)SetWindowLongPtrW(hButton, GWLP_WNDPROC, (LONG_PTR)ButtonProc);
    SetProp(hButton, L"OldProc", (HANDLE)oldProc);


    ButtonDetails* data = new ButtonDetails{bkcolor,txtcolor, hovercolor,0,x,y};
    SetProp(hButton, L"ButtonDetails", (HANDLE)data);
    return hButton;
}


class ButtonRect{
    public:
        RECT r;
        std::wstring text;
        int messageId;
        int isHover = 0;
        
        ButtonRect(int message_id){
            // message id the id it will send to parent when button is clicked
            messageId = message_id;
        }

        void draw(HDC hdc,int x,int y,int width,int height,std::wstring text_,COLORREF bkcolor,COLORREF txtcolor){
            HBRUSH bg = CreateSolidBrush(bkcolor);
            r.left = x;
            r.top = y;
            r.right = x + width;
            r.bottom = y + height;
            text = text_;
            FillRect(hdc,&r,bg);
            SetTextColor(hdc,txtcolor);
            TextOut(hdc, r.left + 20 , r.top+7 ,text.c_str(), wcslen(text.c_str()));
            DeleteObject(bg);
        }

        bool hover(POINT& p){
            return (p.x >= r.left && p.x <= r.right && p.y >= r.top && p.y <= r.bottom);
        }

        int click(POINT& p,HWND* parent){
            //why hwnd* because if hwnd is null at beginning then null will be copied insted pass address
            if (hover(p)){
                SendMessage(*parent,WM_COMMAND,messageId,0);
                return 1;
            }
            return 0;
        }
};

// class Button_{
//     public:
//         const wchar_t* text;
//         RECT rect;
//         COLORREF bkcolor,txtcolor; //normal state
//         COLORREF bkhover,txthover; //hover colors
//         COLORREF bkclick,txtclick; //clicked colors
//         int isClicked = 0,isHovering = 0;

//         Button_(const wchar_t* text_,int x_,int y_,int width_,int height_,COLORREF bkcolor_,COLORREF txtcolor_,COLORREF bkhover_,COLORREF txthover_,COLORREF bkclick_,COLORREF txtclick_){
//             text = text_;
//             rect = {x_,y_,x_+width_,y_+height_};
//             // x = x_; y = y_;
//             // width = width_;height = height_;
//             bkcolor = bkcolor_; txtcolor = txtcolor_;
//             bkhover = bkhover_; txthover = txthover_;
//             bkclick = bkclick_; txtclick = txtclick_;
//         }

//         void draw(HDC hdc,int txtX,int txtY,int fontHeight,bool Italics = false,bool UnderLine = FALSE,bool hovering = FALSE,bool clicked = false){
//             //txtX,txtY are x,y coordinates of font
//             // RECT r{x,y,x+width,y+width};
//             HBRUSH brush;
//             HFONT font = CreateFont(-fontHeight, 0, 0, 0,560,Italics, UnderLine, FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
//                 DEFAULT_QUALITY,FIXED_PITCH | FF_MODERN,L"Consolas");

//             COLORREF textColor,bgColor;
//             if (isHovering){
//                 bgColor = bkhover;
//                 textColor = txthover;
//             }
//             else if (isClicked){
//                 bgColor = bkclick;
//                 textColor = txtclick;
//             }
//             else{
//                 bgColor = bkcolor;
//                 textColor = txtcolor;
//             }

//             brush = CreateSolidBrush(bgColor);

//             // RECT r{400,0,450,30};
//             FillRect(hdc, &rect, brush);
//             SelectObject(hdc, font);

//             SetTextColor(hdc, textColor);
//             SetBkMode(hdc, TRANSPARENT);

//             // TextOut(hdc, rect.left + txtX, rect.top + txtY, text, wcslen(text));
//             TextOut(hdc, rect.left + 0, rect.top + 0, text, wcslen(text));

//             DeleteObject(brush);
//             DeleteObject(font);
//         }

//         int isHover(int x,int y){
//             std::cout << x  << " " << y;
//             //x,y are relative points of mouse in the window where the button must be drawn
//             return PtInRect(&rect, { x, y });
//         }
// };

#endif