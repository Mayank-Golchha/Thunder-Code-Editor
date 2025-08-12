// #ifndef CUSTOMWINDOW_H
// #define CUSTOMWINDOW_H

// #include <wchar.h>
// #include <windows.h>

// #include "dimensions.h"


// struct CustomWindowDetails{
//     HFONT font;
//     COLORREF bkcolor;
//     COLORREF txtcolor;
//     COLORREF hovercolorbk;
//     COLORREF hovercolortc;
//     int hovering;
// };

// LRESULT CALLBACK CustomWindowProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp){
//     CustomWindowDetails* data = (CustomWindowDetails*)GetProp(hWnd, L"CustomWindowDetails");

//     if (msg == WM_PAINT){
//         PAINTSTRUCT ps;
//         HBRUSH brush;
        
//         if (data->hovering) brush = CreateSolidBrush(data->hovercolorbk);
//         else brush = CreateSolidBrush(data->bkcolor);
        
//         HDC hdc = BeginPaint(hWnd, &ps);

//         RECT rc;
//         GetClientRect(hWnd,&rc);

//         FillRect(hdc, &rc, brush);

//         SetBkMode(hdc, TRANSPARENT);

//         if (data->font){
//             SelectObject(hdc, data->font);
//         }

//         if (data->hovering) SetTextColor(hdc, data->hovercolortc);
//         else SetTextColor(hdc, data->txtcolor);

//         wchar_t buf[256];
//         GetWindowTextW(hWnd, buf, 256);
//         // DrawTextW(hdc, buf, -1, &rc, DT_VCENTER | DT_SINGLELINE);
//         TextOut(hdc, 5, 1,buf, wcslen(buf));


//         DeleteObject(brush);  // Clean up
//         EndPaint(hWnd, &ps);
//         return 0;
//     }

//     else if (msg == WM_SETFONT){
//         // hFont = (HFONT)wp;
//         if (LOWORD(lp)) InvalidateRect(hWnd, NULL, TRUE);
//         return 0;
//     }

//     else if (msg == WM_CREATE) SendMessage(hWnd, WM_SETFONT, (WPARAM)data->font, TRUE);
    
//     else if (msg == WM_MOUSEMOVE){
//         if (!data->hovering){
//             TRACKMOUSEEVENT tme = { sizeof(tme) };
//             tme.dwFlags = TME_LEAVE;
//             tme.hwndTrack = hWnd;
//             TrackMouseEvent(&tme);
//             data->hovering = 1;
                    
//             InvalidateRect(hWnd,NULL,TRUE);
//         }
//     }

//     else if (msg == WM_MOUSELEAVE){
//         data->hovering = 0;
//         InvalidateRect(hWnd,NULL,TRUE);
//     }

//     return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA), hWnd, msg, wp, lp);
// }


// HWND customWindow(const wchar_t* text,int x,int y,int width,int height,HWND parent,COLORREF bkcolor,COLORREF txtcolor,COLORREF hoverbk,COLORREF hovertc,HFONT font){
//     HWND w = CreateWindowEx(0,L"static",text,WS_VISIBLE | WS_CHILD | WS_BORDER,x,y,width,height,parent,NULL,NULL,NULL);

//     SetWindowLongPtrW(w, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtrW(w, GWLP_WNDPROC, (LONG_PTR)CustomWindowProc));

//     CustomWindowDetails* data = new CustomWindowDetails{font,bkcolor,txtcolor,hoverbk,hovertc,0};
//     SetProp(w, L"CustomWindowDetails", (HANDLE)data);
//     return w;
// }

// #endif





// the content is no copyable
// TODO : add scrolling





#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include <windows.h>

#include "dimensions.h"



struct TextColorStructure{
    std::wstring text;
    int startX;
    int lineNo;
    int endX;
    COLORREF bkcolor; // background color
    COLORREF txtcolor; // text color
};

LRESULT CALLBACK CustomWindowProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp){
    // std::vector<TextColorStructure*>* content = (std::vector<TextColorStructure*>*)GetProp(hWnd,L"TextColors");
    // POINT* scrolledData = (POINT*) GetProp(hWnd,L"scrolled");

    switch (msg){
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT r;
            GetClientRect(hWnd, &r);

            POINT p;
            GetCursorPos(&p);
            ScreenToClient(hWnd,&p);
            
            // HDC memDC = CreateCompatibleDC(hdc);
            // HBITMAP memBitmap = CreateCompatibleBitmap(hdc, r.right, r.bottom);
            // HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            HFONT font = CreateFont(-CONTEXT_MENU_FONT_HEIGHT, 0, 0, 0,
                500,FALSE, FALSE, FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH | FF_MODERN,L"Consolas"
            );

            // SelectObject(memDC,font);
            SelectObject(hdc,font);
        
            COLORREF* bkcolor = (COLORREF*) GetProp(hWnd,L"backgroundcolor");
            HBRUSH bg = CreateSolidBrush(*bkcolor);
            // FillRect(memDC,&r,bg);
            FillRect(hdc,&r,bg);

            // RemoveProp(hWnd,L"backgroundcolor");

            // int x = -scrolledData->x;
            // int y = -scrolledData->y;

            SIZE sz;
            // GetTextExtentPoint32(memDC, L"a", 1, &sz); //choosed "a" because every character will have same width
            GetTextExtentPoint32(hdc, L"a", 1, &sz); //choosed "a" because every character will have same width

            // for (int i = 0;i<content->size();i++){
                // if (!(*content)[i]->startX) x = 0;
                // SetTextColor(hdc,(*content)[i]->txtcolor);

                // SetBkMode(hdc, OPAQUE);  // background drawing
                // SetBkColor(hdc, (*content)[i]->bkcolor); 

                // const wchar_t* wstr = (*content)[i]->text.c_str();
                // TextOut(hdc,x,y + (*content)[i]->lineNo*20,wstr,wcslen(wstr));
                // SetBkMode(hdc, TRANSPARENT);  // background drawing

                
                // x = (*content)[i]->endX * sz.cx;
            // }
            // DeleteObject(bg);
            // DeleteObject(font);
           
            
            // DeleteObject(bg);
            // EndPaint(hWnd, &ps);
            return 0;
        }

        case WM_DESTROY:
        {
            // COLORREF* bkcolor = (COLORREF*) GetProp(hWnd,L"backgroundcolor");
            // RemoveProp(hWnd,L"backgroundcolor");
            // delete bkcolor;
            // for (int i = 0;i<content->size();i++){
                // delete (*content)[i];
            // }
            // delete scrolledData;
            return 0;
        }
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}


HWND createCustomWindow(int x,int y,int maxWidth,int maxHeight,HWND parent,std::vector<TextColorStructure*>* content,COLORREF bkcolor = RGB(0,0,0)){
    // maxwidth and maxheight is the max dimension of this window
    // maxwidth is the default width and maxheight is not the default height
    int h = (*content)[content->size()-1]->lineNo;
    int height = (h+1)*20;
    if (h*20 > maxHeight) height = maxHeight;

    
    std::cout << "width : " << maxWidth << " height : " << height << "\n";
    HWND w = CreateWindowEx(0,L"CustomStaticWindow",L"",WS_POPUP | WS_VISIBLE | WS_BORDER,x,y,maxWidth,height,parent,NULL,NULL,NULL);

    POINT* scrolled = new POINT{0,0};

    SetProp(w, L"TextColors", (HANDLE)content);
    SetProp(w, L"scrolled", (HANDLE)scrolled);
    // SetProp(w, L"backgroundColor", (HANDLE)&bkcolor);
    
    COLORREF* pColor = new COLORREF(bkcolor);
    SetProp(w, L"backgroundColor", (HANDLE)pColor);

    return w;
}


// TextColorStructure* ca = new TextColorStructure{L"Hello workd",0,0,11,RGB(0,0,0),RGB(200,200,200)};
// TextColorStructure* cb = new TextColorStructure{L"Hello workd",0,1,11,RGB(0,0,0),RGB(200,200,200)};
// TextColorStructure* cc = new TextColorStructure{L"Hello workd",0,2,11,RGB(0,0,0),RGB(200,200,200)};
// TextColorStructure* cd = new TextColorStructure{L"Hello workd",0,3,11,RGB(0,0,0),RGB(200,200,200)};

// std::vector<TextColorStructure*>* con = new std::vector<TextColorStructure*>{ca,cb,cc,cd};

// createCustomWindow(100,100,100,300,hmainWindow,con,RGB(0,0,0));


#endif