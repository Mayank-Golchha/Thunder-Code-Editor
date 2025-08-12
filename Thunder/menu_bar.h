#include <windows.h>
#include <vector>
#include <string>

#include "dimensions.h"
#include "colors.h"
#include "string_operations.h"


LRESULT CALLBACK MenuWindowProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp){
    std::vector<std::string> *data = (std::vector<std::string>*)GetProp(hWnd,L"FileTabPtr");
    switch (msg){
        case WM_PAINT:
        {
            PAINTSTRUCT ps;

            HBRUSH brush = CreateSolidBrush(colorTheme.filetabbk);
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT rc,item_rect;
            GetClientRect(hWnd,&rc);
            item_rect.left = rc.left;
            item_rect.top = rc.top;
            item_rect.bottom = SUB_MENU_HEIGHT;
            item_rect.right = rc.right;

            FillRect(hdc, &rc, brush);

            SetBkMode(hdc, TRANSPARENT);

            SetTextColor(hdc, colorTheme.filetabtc);
            brush = CreateSolidBrush(colorTheme.filetabbk);

            for (int i = 0;i<data->size();i++){
                item_rect.top = i*SUB_MENU_HEIGHT;
                item_rect.bottom = item_rect.top + SUB_MENU_HEIGHT;
                std::wstring d = string_to_wstring((*data)[i]);
                FillRect(hdc,&item_rect,brush);
                TextOut(hdc,rc.left,rc.top+i*SUB_MENU_HEIGHT,d.c_str(),wcslen(d.c_str()));
                // DrawTextW(hdc, buf, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }

            // DrawLine(hdc,0,0,rc.right,0,4,colorTheme.hoverwindowhighlightline);

            DeleteObject(brush);  // Clean up
            EndPaint(hWnd, &ps);
            return 0;
        }
        case WM_DESTROY:
        {
            RemoveProp(hWnd,L"FileTabPtr");
            DestroyWindow(hWnd);
            return 0;
        }
    }
}



class MenuBarItem{
    public:
        HWND hWnd;
        const wchar_t* text;
        RECT rect;
        COLORREF bkcolor,txtcolor; //normal state
        COLORREF bkhover,txthover; //hover colors
        COLORREF bkclick,txtclick; //clicked colors
        int isClicked = 0,isHovering = 0;

        MenuBarItem(HWND hWnd_,const wchar_t* text_,int x_,int y_,int width_,int height_,COLORREF bkcolor_,COLORREF txtcolor_,COLORREF bkhover_,COLORREF txthover_,COLORREF bkclick_,COLORREF txtclick_){
            text = text_;
            rect = {x_,y_,x_+width_,y_+height_};
            // x = x_; y = y_;
            // width = width_;height = height_;
            bkcolor = bkcolor_; txtcolor = txtcolor_;
            bkhover = bkhover_; txthover = txthover_;
            bkclick = bkclick_; txtclick = txtclick_;
            hWnd = hWnd_;
        }

        void draw(HDC hdc,int txtX,int txtY,int fontHeight,bool Italics = false,bool UnderLine = FALSE,bool hovering = FALSE,bool clicked = false){
            //txtX,txtY are x,y coordinates of font
            // RECT r{x,y,x+width,y+width};
            HBRUSH brush;
            HFONT font = CreateFont(-fontHeight, 0, 0, 0,560,Italics, UnderLine, FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,FIXED_PITCH | FF_MODERN,L"Consolas");

            COLORREF textColor,bgColor;
            if (isHovering){
                bgColor = bkhover;
                textColor = txthover;
            }
            else if (isClicked){
                bgColor = bkclick;
                textColor = txtclick;
            }
            else{
                bgColor = bkcolor;
                textColor = txtcolor;
            }

            brush = CreateSolidBrush(bgColor);

            // RECT r{400,0,450,30};
            FillRect(hdc, &rect, brush);
            SelectObject(hdc, font);

            SetTextColor(hdc, textColor);
            SetBkMode(hdc, TRANSPARENT);

            // TextOut(hdc, rect.left + txtX, rect.top + txtY, text, wcslen(text));
            TextOut(hdc, rect.left + 0, rect.top + 0, text, wcslen(text));

            DeleteObject(brush);
            DeleteObject(font);
        }

        int isHover(int x,int y){
            //x,y are relative points of mouse in the window where the button must be drawn
            return PtInRect(&rect, { x, y });
        }

        HWND createMenuSubItems(std::vector<std::string> items){
            HWND container = CreateWindowEx(0,L"MenuWindowClass",L"",WS_POPUP | WS_VISIBLE,rect.left,rect.bottom,SUB_MENU_WIDTH,SUB_MENU_HEIGHT*items.size(),hWnd,NULL,NULL,NULL);
            SetProp(container, L"FileTabPtr", (HANDLE)&items);

        }
};