// // // #include <windows.h>

// // // #define TITLEBAR_HEIGHT 30

// // // LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// // // int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
// // //     WNDCLASS wc = {};
// // //     wc.lpfnWndProc = WindowProc;
// // //     wc.hInstance = hInstance;
// // //     wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
// // //     wc.lpszClassName = L"CustomWindowClass";
// // //     wc.hCursor = LoadCursor(NULL,IDC_ARROW);


// // //     RegisterClass(&wc);

// // //     HWND hwnd = CreateWindowEx(
// // //         0,
// // //         wc.lpszClassName,
// // //         L"Borderless Window",
// // //         WS_POPUP | WS_THICKFRAME, // thickframe allows resizing
// // //         100, 100, 800, 600,
// // //         nullptr, nullptr, hInstance, nullptr);

// // //     ShowWindow(hwnd, nCmdShow);
// // //     UpdateWindow(hwnd);

// // //     MSG msg = {};
// // //     while (GetMessage(&msg, nullptr, 0, 0)) {
// // //         TranslateMessage(&msg);
// // //         DispatchMessage(&msg);
// // //     }

// // //     return 0;
// // // }

// // // void DrawTitleBar(HDC hdc, RECT rc) {
// // //     HBRUSH brush = CreateSolidBrush(RGB(255, 140, 0));
// // //     RECT title = { rc.left, rc.top, rc.right, rc.top + TITLEBAR_HEIGHT };
// // //     FillRect(hdc, &title, brush);
// // //     SetBkMode(hdc, TRANSPARENT);
// // //     SetTextColor(hdc, RGB(255, 255, 255));
// // //     title.left += 10;
// // //     title.top += 5;
// // //     DrawText(hdc, L"Custom Title Bar", -1, &title, DT_SINGLELINE | DT_LEFT);
// // //     DeleteObject(brush);
// // // }

// // // LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
// // //     switch (msg) {
// // //     case WM_NCCALCSIZE:
// // //         if (wParam) return 0; // Remove default frame
// // //         break;

// // //     case WM_NCHITTEST: {
// // //         POINT pt = { LOWORD(lParam), HIWORD(lParam) };
// // //         ScreenToClient(hwnd, &pt);
// // //         RECT rc;
// // //         GetClientRect(hwnd, &rc);
// // //         const int border = 8;

// // //         if (pt.y < border) return HTTOP;
// // //         if (pt.y >= rc.bottom - border) return HTBOTTOM;
// // //         if (pt.x < border) return HTLEFT;
// // //         if (pt.x >= rc.right - border) return HTRIGHT;
// // //         if (pt.y < TITLEBAR_HEIGHT) return HTCAPTION; // draggable title bar area
// // //         return HTCLIENT;
// // //     }

// // //     case WM_ERASEBKGND:
// // //         return 1; // prevent flickering

// // //     case WM_PAINT: {
// // //         PAINTSTRUCT ps;
// // //         HDC hdc = BeginPaint(hwnd, &ps);
// // //         RECT rc;
// // //         GetClientRect(hwnd, &rc);
// // //         FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
// // //         DrawTitleBar(hdc, rc);
// // //         EndPaint(hwnd, &ps);
// // //         return 0;
// // //     }

// // // case WM_NCPAINT:
// // //     return 0;

// // // case WM_NCACTIVATE:
// // //     return TRUE;


// // //     case WM_LBUTTONDOWN:
// // //         SetCapture(hwnd);
// // //         break;
// // //     case WM_LBUTTONUP:
// // //         ReleaseCapture();
// // //         break;
// // //     case WM_DESTROY:
// // //         PostQuitMessage(0);
// // //         return 0;
// // //     }

// // //     return DefWindowProc(hwnd, msg, wParam, lParam);
// // // }






// // // // TODO : handle hover animation
// // // // TODO : handle resizing
// // // // TODO : handle horizontal scrolling
// // // // if tabs are more than the width then the tab will not be displayed
// // // #ifndef FILETAB_H
// // // #define FILETAB_H

// // // #include <wchar.h>
// // // #include <iostream>
// // // #include <vector>

// // // #include "colors.h"
// // // #include "window_handles.h"
// // // #include "string_operations.h"
// // // #include "dimensions.h"
// // // #include "font.h"

// // // int activeFileTabId = -1;
// // // int totalTabs = 0;

// // // struct FileTabDetails{
// // //     int hover;
// // //     int click;
// // //     int Id; //tab
// // //     HFONT font;
// // // };

// // // struct FileTab{
// // //     std::string FileName;
// // //     std::string FilePath;
// // // };

// // // std::vector<FileTab*> fileTabs;
// // // std::vector<FileTabDetails*> fileTabsDetails;

// // // struct FileTab* a = new FileTab{"Computer","asd"}; 
// // // struct FileTab* b = new FileTab{"World","asd"}; 
// // // struct FileTab* c = new FileTab{"hi","asd"}; 
// // // struct FileTab* d = new FileTab{"i am","asd"}; 
// // // struct FileTab* e = new FileTab{"Mayank","asd"}; 
// // // struct FileTab* f = new FileTab{"Golchha","asd"}; 
// // // struct FileTab* g = new FileTab{"a","asd"}; 
// // // struct FileTab* hi = new FileTab{"Computer","asd"}; 
// // // struct FileTab* i = new FileTab{"Science","asd"}; 
// // // struct FileTab* j = new FileTab{"Student","asd"}; 

// // // HWND createFileTabContainer(HWND);


// // // LRESULT CALLBACK FileTabContainerProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
// // //     switch (msg) {
// // //         case WM_PAINT:
// // //             {
// // //                 PAINTSTRUCT ps;

// // //                 HBRUSH brush = CreateSolidBrush(colorTheme.filetabContainer);
// // //                 HDC hdc = BeginPaint(hWnd, &ps);

// // //                 RECT rc;
// // //                 GetClientRect(hWnd,&rc);

// // //                 FillRect(hdc, &rc, brush);

// // //                 DeleteObject(brush);  // Clean up
// // //                 EndPaint(hWnd, &ps);
// // //                 return 0;
// // //             }
// // //     }

// // //     return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA), hWnd, msg, wp, lp);

// // // }

// // // LRESULT CALLBACK FileTabProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp){
// // //     FileTabDetails* data = (FileTabDetails*)GetProp(hWnd, L"FileTabDetails");

// // //     switch (msg){
// // //         case WM_LBUTTONDOWN:
// // //             {
// // //                 POINT p;
// // //                 GetCursorPos(&p);
// // //                 ScreenToClient(hWnd,&p);

// // //                 RECT r;
// // //                 GetClientRect(hWnd,&r);
// // //                 if (p.x > r.right-FILE_TAB_CLOSE_BUTTON_WIDTH){
// // //                     RemoveProp(hWnd, L"FileTabDetails");
// // //                     fileTabs.erase(fileTabs.begin() + data->Id);
// // //                     DestroyWindow(hWnd);
// // //                     DestroyWindow(hFileTabContainer);
// // //                     hFileTabContainer = createFileTabContainer(hmainWindow);
// // //                 }
// // //                 else{
// // //                     data->click = 1;
// // //                     activeFileTabId = data->Id;
// // //                     InvalidateRect(hWnd,NULL,TRUE);
// // //                     //write content to editor
// // //                 }
// // //                 break;
// // //             }

// // //         case WM_PAINT:
// // //             {
// // //                 PAINTSTRUCT ps;

// // //                 HBRUSH brush = CreateSolidBrush(colorTheme.filetabbk);
// // //                 HDC hdc = BeginPaint(hWnd, &ps);

// // //                 RECT rc;
// // //                 GetClientRect(hWnd,&rc);

// // //                 int k = (rc.bottom - FILE_TAB_HEIGHT)/2 + 3;

// // //                 FillRect(hdc, &rc, brush);

// // //                 if (data->font){
// // //                     SelectObject(hdc, data->font);
// // //                 }

// // //                 SetBkMode(hdc, TRANSPARENT);

// // //                 SetTextColor(hdc, colorTheme.filetabtc);

// // //                 wchar_t buf[256];
// // //                 GetWindowTextW(hWnd, buf, 256);
                
// // //                 TextOut(hdc, 8, k, buf, wcslen(buf)); //file name

// // //                 TextOut(hdc, rc.right-20, k, L"X", wcslen(L"X"));

// // //                 if (data->click && activeFileTabId == data->Id){
// // //                     DrawLine(hdc,0,0,rc.right,0,2,colorTheme.hoverwindowhighlightline);
// // //                 }


// // //                 DeleteObject(brush);  // Clean up
// // //                 EndPaint(hWnd, &ps);
// // //                 return 0;
// // //             }
// // //         case WM_CREATE:
// // //             SendMessage(hWnd, WM_SETFONT, (WPARAM)data->font, TRUE);
// // //             break;

// // //         case WM_SETFONT:
// // //             if (LOWORD(lp)) InvalidateRect(hWnd, NULL, TRUE);
// // //             return 0;

// // //     }
// // //     return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA), hWnd, msg, wp, lp);
// // // }


// // // HWND createFileTab(HWND hWnd,int x,int y,int width,int height,const wchar_t* text,int Id){
    
// // //     HFONT font = customFont(FILE_TAB_FONT_SIZE,8,FILE_TAB_FONT_WEIGHT,L"Segoe UI");
// // //     HWND tab = CreateWindowEx(0,L"static",text,WS_CHILD | WS_VISIBLE | SS_NOTIFY,x,y,width,height,hWnd,NULL,NULL,NULL);
// // //     SetWindowLongPtrW(tab, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtrW(tab, GWLP_WNDPROC, (LONG_PTR)FileTabProc));
    
// // //     FileTabDetails* data = new FileTabDetails{0,0,Id,font};
    
// // //     SetProp(tab, L"FileTabDetails", (HANDLE)data);
// // //     return tab;
// // // }


// // // HWND createFileTabContainer(HWND hWnd){
// // //     RECT r;
// // //     GetClientRect(hWnd,&r);
// // //     //added 14 with width of container because r.right was not giving the actual width
// // //     int w = r.right + 14;
// // //     hFileTabContainer = CreateWindowEx(0,L"static",L"",WS_CHILD | WS_VISIBLE ,FILE_TAB_X,FILE_TAB_Y,w,FILE_TAB_HEIGHT,hWnd,NULL,NULL,NULL);
// // //     SetWindowLongPtrW(hFileTabContainer, GWLP_USERDATA, (LONG_PTR)SetWindowLongPtrW(hFileTabContainer, GWLP_WNDPROC, (LONG_PTR)FileTabContainerProc));
    
// // //     HFONT font = customFont(FILE_TAB_FONT_SIZE,8,FILE_TAB_FONT_WEIGHT,L"Segoe UI");

// // //     int temp = FILE_TAB_X;
// // //     // int i = 0;
    
// // //     for (FileTab* f : fileTabs){
// // //         std::wstring wtext = string_to_wstring(f->FileName);
// // //         const wchar_t* text = wtext.c_str();
// // //         int width = calculateFontWidth(text,font);
// // //         if (temp + width >= w) break;
// // //         // createFileTab(hFileTabContainer,temp,0,width,FILE_TAB_HEIGHT,text,i++);
// // //         createFileTab(hFileTabContainer,temp,0,width,FILE_TAB_HEIGHT,text,totalTabs++);
// // //         temp += width + 1 ;
// // //     }
// // //     DeleteObject(font);

// // // }

// // // #endif













// // // #include "window_handles.h"
// // // #include "dimensions.h"
// // // #include "colors.h"
// // // #include "editorFunctions.h"
// // // #include "string_operations.h"

// // // #include <vector>
// // // #include <iostream>

// // // using namespace std;

// // // #ifndef WM_MOUSEHWHEEL
// // // #define WM_MOUSEHWHEEL 0x020E
// // // #endif

// // // #define MIN(a,b) (((a) < (b)) ? (a) : (b))
// // // #define MAX(a,b) (((a) > (b)) ? (a) : (b))

// // // const int SCROLLBAR_WIDTH = 10;
// // // float scrollOffset = 0.0f;     // Vertical scroll offset
// // // float scrollOffsetX = 0.0f;    // Horizontal scroll offset

// // // bool draggingVert = false;
// // // bool draggingHorz = false;

// // // int dragStartY = 0;
// // // int dragStartX = 0;

// // // int thumbStartPosVert = 0;
// // // int thumbStartPosHorz = 0;

// // // int totalLines = 1;
// // // int longestLineLength = 0;

// // // bool draggingWindow = false;
// // // POINT dragStartMousePos;
// // // RECT dragStartWindowRect;

// // // wchar_t buf[MAXINPUTBUF]; // edit buffer


// // // int cursor = 0;

// // // int cursorX = 0,cursorY = 0;

// // // std::vector<int> LinesWidth = {1}; //will save widh of all lines


// // // LRESULT CALLBACK CustomEditWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
// // //     RECT rr;
// // //     GetClientRect(hWnd, &rr);

// // //     int contentHeight = MAX(EDITOR_HEIGHT_MAX, EDITOR_LINE_HEIGHT * totalLines);
// // //     int contentWidth  = MAX(rr.right, 10 * longestLineLength);

// // //     switch (msg) {
// // //         case WM_SETFOCUS:
// // //             {
// // //                 POINT p;
// // //                 GetCursorPos(&p);
// // //                 ScreenToClient(hWnd,&p);
// // //                 int ly = p.y/EDITOR_LINE_HEIGHT; //line no

// // //                 //will check for if the selected point is within writing part
// // //                 if (ly < LinesWidth.size() && p.x < LinesWidth[ly]) drawCarret(hWnd,p.x,p.y,CARRET_HEIGHT);
// // //                 else drawCarret(hWnd,cursorX,cursorY,CARRET_HEIGHT);
// // //                 // break;
// // //             }
// // //                     return FALSE;

// // //         case WM_KILLFOCUS:
// // //             HideCaret(hWnd);
// // //             DestroyCaret(); 
// // //             break;
        
// // //         case WM_GETDLGCODE:
// // //             // Sent to the window procedure associated with a control
// // //             // By default, the system handles all keyboard input to the control;
// // //             // the system interprets certain types of keyboard input as dialog box navigation keys.
// // //             // To override this default behavior, the control can respond to the 
// // //             // WM_GETDLGCODE message to indicate the types of input it wants to process itself.
// // //             return DLGC_WANTCHARS | DLGC_WANTARROWS | DLGC_WANTTAB;

// // //         case WM_KEYDOWN:
// // //             {
// // //                 switch (wp){
// // //                     case VK_BACK:
// // //                         //backspace
// // //                         cursor--;
// // //                         removeAtIndex(buf,cursor,MAXINPUTBUF);
// // //                         InvalidateRect(hWnd,NULL,TRUE);
// // //                         break;


// // //                 }
// // //                 break;
// // //             }

// // //         case WM_CHAR:
// // //             if (wp < VK_SPACE) break;
// // //                 // if (wcslen(buf)+1 < MAXINPUTBUF)
// // //                 cout << wp;
// // //                 // {
// // //                     buf[cursor] = wp;
// // //                     cursor++;
// // //                     buf[cursor] = '\0';
// // //                     InvalidateRect(hWnd,NULL,TRUE);
// // //                     // select = cursor;
// // //                 // }
// // //             break;

// // //         case WM_PAINT: {
// // //             PAINTSTRUCT ps;
// // //             HDC hdc = BeginPaint(hWnd, &ps);

// // //             RECT clientRect;
// // //             GetClientRect(hWnd, &clientRect);
// // //             int clientWidth = clientRect.right - clientRect.left;
// // //             int clientHeight = clientRect.bottom - clientRect.top;

// // //             HDC memDC = CreateCompatibleDC(hdc);
// // //             HBITMAP memBitmap = CreateCompatibleBitmap(hdc, clientWidth, clientHeight);
// // //             HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

// // //             // Background
// // //             HBRUSH hBrush = CreateSolidBrush(colorTheme.editorbk);
// // //             FillRect(memDC, &clientRect, hBrush);
// // //             DeleteObject(hBrush);

// // //             // VERTICAL SCROLLBAR
// // //             RECT vTrackRect = { clientRect.right - SCROLLBAR_WIDTH, 0, clientRect.right, clientRect.bottom - SCROLLBAR_WIDTH };
// // //             hBrush = CreateSolidBrush(RGB(230, 230, 230));
// // //             FillRect(memDC, &vTrackRect, hBrush);
// // //             DeleteObject(hBrush);

// // //             float visibleRatioVert = (float)clientHeight / contentHeight;
// // //             int thumbHeight = MAX((int)(visibleRatioVert * clientHeight), 30);
// // //             int maxScrollVert = contentHeight - clientHeight;
// // //             int thumbPosVert = (maxScrollVert > 0) ? (int)(scrollOffset * (clientHeight - thumbHeight) / maxScrollVert) : 0;

// // //             RECT vThumbRect = { clientRect.right - SCROLLBAR_WIDTH, thumbPosVert, clientRect.right, thumbPosVert + thumbHeight };
// // //             hBrush = CreateSolidBrush(RGB(150, 150, 150));
// // //             FillRect(memDC, &vThumbRect, hBrush);
// // //             DeleteObject(hBrush);

// // //             // HORIZONTAL SCROLLBAR
// // //             int scrollableWidth = clientWidth - SCROLLBAR_WIDTH;
// // //             RECT hTrackRect = { 0, clientRect.bottom - SCROLLBAR_WIDTH, scrollableWidth, clientRect.bottom };
// // //             hBrush = CreateSolidBrush(RGB(230, 230, 230));
// // //             FillRect(memDC, &hTrackRect, hBrush);
// // //             DeleteObject(hBrush);

// // //             float visibleRatioHorz = (float)scrollableWidth / contentWidth;
// // //             int thumbWidth = MAX((int)(visibleRatioHorz * scrollableWidth), 30);
// // //             int maxScrollHorz = contentWidth - scrollableWidth;
// // //             int thumbPosHorz = (maxScrollHorz > 0) ? (int)(scrollOffsetX * (scrollableWidth - thumbWidth) / maxScrollHorz) : 0;

// // //             RECT hThumbRect = { thumbPosHorz, clientRect.bottom - SCROLLBAR_WIDTH, thumbPosHorz + thumbWidth, clientRect.bottom };
// // //             hBrush = CreateSolidBrush(RGB(150, 150, 150));
// // //             FillRect(memDC, &hThumbRect, hBrush);
// // //             DeleteObject(hBrush);

// // //             //text drawing
// // //             SetBkMode(memDC, TRANSPARENT);
// // //             TextOut(memDC, 8, 1, buf, wcslen(buf)); 


// // //             BitBlt(hdc, 0, 0, clientWidth, clientHeight, memDC, 0, 0, SRCCOPY);


// // //             SelectObject(memDC, oldBitmap);
// // //             DeleteObject(memBitmap);
// // //             DeleteDC(memDC);

// // //             EndPaint(hWnd, &ps);
// // //             return 0;
// // //         }

// // //         case WM_LBUTTONDOWN: {
// // //             POINT pt = { LOWORD(lp), HIWORD(lp) };
// // //             RECT clientRect;
// // //             GetClientRect(hWnd, &clientRect);

// // //             int clientWidth = clientRect.right - clientRect.left;
// // //             int clientHeight = clientRect.bottom - clientRect.top;
// // //             int scrollableWidth = clientWidth - SCROLLBAR_WIDTH;
// // //             int scrollableHeight = clientHeight - SCROLLBAR_WIDTH;

// // //             // Vertical scrollbar click
// // //             if (pt.x >= clientRect.right - SCROLLBAR_WIDTH && pt.y <= clientRect.bottom - SCROLLBAR_WIDTH) {
// // //                 float visibleRatioVert = (float)clientHeight / contentHeight;
// // //                 int thumbHeight = MAX((int)(visibleRatioVert * clientHeight), 30);
// // //                 int maxScrollVert = contentHeight - clientHeight;
// // //                 int thumbPosVert = (maxScrollVert > 0) ? (int)(scrollOffset * (clientHeight - thumbHeight) / maxScrollVert) : 0;

// // //                 if (pt.y >= thumbPosVert && pt.y <= thumbPosVert + thumbHeight) {
// // //                     draggingVert = true;
// // //                     dragStartY = pt.y;
// // //                     thumbStartPosVert = thumbPosVert;
// // //                     SetCapture(hWnd);
// // //                 }
// // //             }
// // //             // Horizontal scrollbar click
// // //             else if (pt.y >= clientRect.bottom - SCROLLBAR_WIDTH && pt.x <= clientRect.right - SCROLLBAR_WIDTH) {
// // //                 float visibleRatioHorz = (float)scrollableWidth / contentWidth;
// // //                 int thumbWidth = MAX((int)(visibleRatioHorz * scrollableWidth), 30);
// // //                 int maxScrollHorz = contentWidth - scrollableWidth;
// // //                 int thumbPosHorz = (maxScrollHorz > 0) ? (int)(scrollOffsetX * (scrollableWidth - thumbWidth) / maxScrollHorz) : 0;

// // //                 if (pt.x >= thumbPosHorz && pt.x <= thumbPosHorz + thumbWidth) {
// // //                     draggingHorz = true;
// // //                     dragStartX = pt.x;
// // //                     thumbStartPosHorz = thumbPosHorz;
// // //                     SetCapture(hWnd);
// // //                 }
// // //             }
// // //             else {
// // //                 draggingWindow = true;
// // //                 SetCapture(hWnd);

// // //                 POINT screenPt = pt;
// // //                 ClientToScreen(hWnd, &screenPt);
// // //                 dragStartMousePos = screenPt;

// // //                 GetWindowRect(hWnd, &dragStartWindowRect);
// // //             }

// // //             SetFocus(hWnd);
// // //             // PostMessageW(GetParent(hWnd), WM_NEXTDLGCTL, (WPARAM)hWnd, TRUE);


// // //             return 0;
// // //         }

// // //         case WM_MOUSEMOVE: {
// // //             RECT clientRect;
// // //             GetClientRect(hWnd, &clientRect);

// // //             int clientWidth = clientRect.right - clientRect.left;
// // //             int clientHeight = clientRect.bottom - clientRect.top;
// // //             int scrollableWidth = clientWidth - SCROLLBAR_WIDTH;
// // //             int scrollableHeight = clientHeight - SCROLLBAR_WIDTH;

// // //             if (draggingVert) {
// // //                 float visibleRatioVert = (float)clientHeight / contentHeight;
// // //                 int thumbHeight = MAX((int)(visibleRatioVert * clientHeight), 30);
// // //                 int maxScrollVert = contentHeight - clientHeight;
// // //                 int maxThumbMove = clientHeight - thumbHeight;

// // //                 int y = (short)HIWORD(lp);
// // //                 int deltaY = y - dragStartY;

// // //                 int newThumbPos = MAX(0, MIN(maxThumbMove, thumbStartPosVert + deltaY));
// // //                 scrollOffset = (float)newThumbPos * maxScrollVert / maxThumbMove;
// // //                 scrollOffset = MAX(0.0f, MIN(scrollOffset, (float)maxScrollVert));

// // //                 InvalidateRect(hWnd, NULL, FALSE);
// // //             }
// // //             else if (draggingHorz) {
// // //                 float visibleRatioHorz = (float)scrollableWidth / contentWidth;
// // //                 int thumbWidth = MAX((int)(visibleRatioHorz * scrollableWidth), 30);
// // //                 int maxScrollHorz = contentWidth - scrollableWidth;
// // //                 int maxThumbMove = scrollableWidth - thumbWidth;

// // //                 int x = (short)LOWORD(lp);
// // //                 int deltaX = x - dragStartX;

// // //                 int newThumbPos = MAX(0, MIN(maxThumbMove, thumbStartPosHorz + deltaX));
// // //                 scrollOffsetX = (float)newThumbPos * maxScrollHorz / maxThumbMove;
// // //                 scrollOffsetX = MAX(0.0f, MIN(scrollOffsetX, (float)maxScrollHorz));

// // //                 InvalidateRect(hWnd, NULL, FALSE);
// // //             }
// // //             else if (draggingWindow) {
// // //                 POINT pt = { LOWORD(lp), HIWORD(lp) };
// // //                 POINT screenPt = pt;
// // //                 ClientToScreen(hWnd, &screenPt);

// // //                 int dx = screenPt.x - dragStartMousePos.x;
// // //                 int dy = screenPt.y - dragStartMousePos.y;

// // //                 MoveWindow(hWnd,
// // //                     dragStartWindowRect.left + dx,
// // //                     dragStartWindowRect.top + dy,
// // //                     dragStartWindowRect.right - dragStartWindowRect.left,
// // //                     dragStartWindowRect.bottom - dragStartWindowRect.top,
// // //                     TRUE);
// // //             }
// // //             return 0;
// // //         }

// // //         case WM_LBUTTONUP: {
// // //             if (draggingVert || draggingHorz || draggingWindow) {
// // //                 draggingVert = false;
// // //                 draggingHorz = false;
// // //                 draggingWindow = false;
// // //                 ReleaseCapture();
// // //             }
// // //             return 0;
// // //         }

// // //         case WM_MOUSEWHEEL: {
// // //             RECT clientRect;
// // //             GetClientRect(hWnd, &clientRect);

// // //             int clientWidth = clientRect.right - clientRect.left;
// // //             int clientHeight = clientRect.bottom - clientRect.top;
// // //             int scrollableWidth = clientWidth - SCROLLBAR_WIDTH;
// // //             int scrollableHeight = clientHeight - SCROLLBAR_WIDTH;

// // //             if (GetKeyState(VK_SHIFT) & 0x8000) {
// // //                 int maxScrollHorz = contentWidth - scrollableWidth;
// // //                 int delta = GET_WHEEL_DELTA_WPARAM(wp);

// // //                 scrollOffsetX -= delta / (float)WHEEL_DELTA * 50.0f;
// // //                 scrollOffsetX = MAX(0.0f, MIN(scrollOffsetX, (float)maxScrollHorz));
// // //             }
// // //             else {
// // //                 int maxScrollVert = contentHeight - clientHeight;
// // //                 int delta = GET_WHEEL_DELTA_WPARAM(wp);

// // //                 scrollOffset -= delta / (float)WHEEL_DELTA * 50.0f;
// // //                 scrollOffset = MAX(0.0f, MIN(scrollOffset, (float)maxScrollVert));
// // //             }

// // //             InvalidateRect(hWnd, NULL, FALSE);
// // //             return 0;
// // //         }

// // //         case WM_MOUSEHWHEEL: {
// // //             RECT clientRect;
// // //             GetClientRect(hWnd, &clientRect);

// // //             int clientWidth = clientRect.right - clientRect.left;
// // //             int scrollableWidth = clientWidth - SCROLLBAR_WIDTH;
// // //             int maxScrollHorz = contentWidth - scrollableWidth;
// // //             int delta = GET_WHEEL_DELTA_WPARAM(wp);

// // //             scrollOffsetX -= delta / (float)WHEEL_DELTA * 50.0f;
// // //             scrollOffsetX = MAX(0.0f, MIN(scrollOffsetX, (float)maxScrollHorz));

// // //             InvalidateRect(hWnd, NULL, FALSE);
// // //             return 0;
// // //         }


// // //         case WM_SIZE: {
// // //             InvalidateRect(hWnd, NULL, FALSE);
// // //             return 0;
// // //         }

// // //         case WM_ERASEBKGND:
// // //             return 1;
            

// // //         case WM_DESTROY:
// // //             PostQuitMessage(0);
// // //             return 0;
// // //     }

// // //     return DefWindowProc(hWnd, msg, wp, lp);
// // // }

// // // void createEditor(HWND hWnd, int x, int y, int width, int height) {
// // //     // HWND d = CreateWindowEx(0, L"static", L"",
// // //         // WS_VISIBLE | WS_CHILD | WS_BORDER, x, y, width, height,
// // //         // hWnd, NULL, NULL, NULL);

// // //     HWND hwnd = CreateWindowEx(0, L"CustomEditWindowClass", L"",
// // //         WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP, x, y, width, height,
// // //         hWnd, NULL, NULL, NULL);
// // // }




// // //editor

// // // else if (p.x > EDITOR_WRITING_PART_X){
// //             //     //writing part
// //             //     HDC hdc = GetDC(hWnd);
// //             //     SelectObject(hdc, editorFont);
// //             //     float topY = scrolledYCoordinate(vertThumbY,thumbVertHeight,r.bottom - EDITOR_WRITING_PART_Y - SCROLL_BAR_WIDTH); //cordinate to top window
// //             //     int ln = (topY + p.y - EDITOR_WRITING_PART_Y)/EDITOR_LINE_HEIGHT;
// //             //     cursorY = ln;
// //             //     if (cursorY >= editContent.size()) cursorY = editContent.size() - 1;
// //             //     if (cursorY < 0) cursorY = 0;

// //             //     int relativeX = p.x - EDITOR_WRITING_PART_X;
// //             //     cursorX = 0;
// //             //     int s = 0;

// //             //     for (int i = 0;i<editContent[cursorY].size();i++){
// //             //         SIZE sz;
// //             //         GetTextExtentPoint32(hdc, &editContent[cursorY][i], 1, &sz);
// //             //         s += sz.cx;
// //             //         if (s > relativeX) {
// //             //             cursorX = i;
// //             //             break;
// //             //         }
// //             //         cursorX = i + 1;
// //             //     }
                
                
// //             //     SIZE sz;
// //             //     GetTextExtentPoint32(hdc, editContent[cursorY].c_str(), cursorX, &sz);
// //             //     ReleaseDC(hWnd,hdc);

// //             //     SetCaretPos(sz.cx + EDITOR_WRITING_PART_X, cursorY * EDITOR_LINE_HEIGHT + EDITOR_WRITING_PART_Y);
                
// //             // }








// // // //displaying two extra line so that clipped part will be seen and there will be no gap at bottom
// //             // for (int i = ln; i < ln + totalLines + 2 && i < editContent.size(); i++) {
// //             //     editContent[i] = L"hello worldworldworld" + string_to_wstring(std::to_string(i));

// //             //     // int drawY = EDITOR_WRITING_PART_Y + (i - ln) * EDITOR_LINE_HEIGHT - offsetY;
// //             //     int drawY = (i - ln) * EDITOR_LINE_HEIGHT + topY;
// //             //     std::wstring line = editContent[i];
// //             //     if (x_ln < line.size())
// //             //         line = line.substr(x_ln);
// //             //     else
// //             //         line = L"";

// //             //     //will color the selected lines
// //             //     if (inSelectedLineVector(&selectedLines,i) >= 0) ColorSelectedLine(memDC,EDITOR_WRITING_PART_X,drawY,lineMaxWidth);
                
// //             //     SetTextColor(memDC, BLACK);

// //             //     TextOut(memDC, EDITOR_WRITING_PART_X, drawY, line.c_str(), wcslen(line.c_str()));

// //             //     drawLineNo(memDC,EDITOR_X,drawY,i,editorFont);

// //             // }










// // // cd "d:\C Projects\c++\Text Editor\" ; if ($?) { g++ main.cpp -lgdi32 -o main } ; if ($?) { .\main }
// // #pragma comment(lib, "msimg32.lib")

// // #ifndef UNICODE
// // #define UNICODE
// // #endif 

// // #include <windows.h>

// // #include <wingdi.h>

// // float t = 0.0f;       // blend factor from 0 to 1
// // bool increasing = true;


// // void DrawGradientRect(HDC hdc, RECT r, COLORREF topleftColor, COLORREF toprightColor,COLORREF bottomleftColor,COLORREF bottomrightColor) {
// //     TRIVERTEX vertex[4] = {
// //             {r.left, r.top, (COLOR16)(GetRValue(topleftColor) << 8), (COLOR16)(GetGValue(topleftColor) << 8), (COLOR16)(GetBValue(topleftColor) << 8), 0x0000},    // Top-left (blue)
// //             {r.right, r.top,(COLOR16)(GetRValue(toprightColor) << 8),(COLOR16)(GetGValue(toprightColor) << 8), (COLOR16)(GetBValue(toprightColor) << 8), 0x0000},    // Top-right (red)
// //             {r.left, r.bottom,(COLOR16)(GetRValue(bottomleftColor) << 8), (COLOR16)(GetGValue(bottomleftColor) << 8), (COLOR16)(GetBValue(bottomleftColor) << 8), 0x0000},    // Bottom-left (green)
// //             {r.right, r.bottom,(COLOR16)(GetRValue(bottomrightColor) << 8), (COLOR16)(GetGValue(bottomrightColor) << 8), (COLOR16)(GetBValue(bottomrightColor) << 8), 0x0000}     // Bottom-right (yellow)
// //         };

// //     GRADIENT_TRIANGLE triangles[2] = {
// //         {0, 1, 2}, // Top-left, top-right, bottom-left
// //         {2, 1, 3}  // Bottom-left, top-right, bottom-right
// //     };

// //         // Draw the gradient
// //     GradientFill(hdc, vertex, 4, triangles, 2, GRADIENT_FILL_TRIANGLE);
// // }

// // // void DrawGradientLine(HDC hdc, int x1, int y, int x2, COLORREF startColor, COLORREF endColor) {
// // //     int width = x2 - x1;

// // //     BYTE r1 = GetRValue(startColor);
// // //     BYTE g1 = GetGValue(startColor);
// // //     BYTE b1 = GetBValue(startColor);

// // //     BYTE r2 = GetRValue(endColor);
// // //     BYTE g2 = GetGValue(endColor);
// // //     BYTE b2 = GetBValue(endColor);

// // //     for (int i = 0; i < width; ++i) {
// // //         float t = (float)i / width;

// // //         BYTE r = (BYTE)(r1 + t * (r2 - r1));
// // //         BYTE g = (BYTE)(g1 + t * (g2 - g1));
// // //         BYTE b = (BYTE)(b1 + t * (b2 - b1));

// // //         SetPixel(hdc, x1 + i, y, RGB(r, g, b));
// // //     }
// // // }

// // // void DrawChangingGradientLine(HDC hdc, int x1, int y, int x2, float t) {
// // //     int width = x2 - x1;

// // //     // Gradient from red to blue to green and loop
// // //     COLORREF startColor = RGB(255, 0, 0); // Red
// // //     COLORREF endColor = RGB(0, 0, 255);   // Blue

// // //     if (t > 1.0f) t = 1.0f;
// // //     if (t < 0.0f) t = 0.0f;

// // //     BYTE r = (BYTE)((1.0f - t) * 255);
// // //     BYTE g = 0;
// // //     BYTE b = (BYTE)(t * 255);

// // //     for (int i = 0; i < width; ++i) {
// // //         float segmentT = (float)i / width;

// // //         BYTE rStep = (BYTE)(r * (1.0f - segmentT));
// // //         BYTE bStep = (BYTE)(b * segmentT);

// // //         SetPixel(hdc, x1 + i, y, RGB(rStep, g, bStep));
// // //         SetPixel(hdc, x1 + i, y+1, RGB(rStep, g, bStep));
// // //     }
// // // }

// // void DrawBlurredLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
// //     // Draw the main line
// //     HPEN hPen = CreatePen(PS_SOLID, 1, color);
// //     HPEN old = (HPEN)SelectObject(hdc, hPen);
// //     MoveToEx(hdc, x1, y1, NULL);
// //     LineTo(hdc, x2, y2);
// //     SelectObject(hdc, old);
// //     DeleteObject(hPen);

// //     // Simulate blur with faded lines
// //     for (int i = -2; i <= 2; i++) {
// //         if (i == 0) continue;
// //         COLORREF faded = RGB(GetRValue(color)/2, GetGValue(color)/2, GetBValue(color)/2);
// //         hPen = CreatePen(PS_SOLID, 1, faded);
// //         old = (HPEN)SelectObject(hdc, hPen);
// //         MoveToEx(hdc, x1 + i, y1 + i, NULL);
// //         LineTo(hdc, x2 + i, y2 + i);
// //         SelectObject(hdc, old);
// //         DeleteObject(hPen);
// //     }
// // }

// #ifndef UNICODE
// #define UNICODE
// #endif 

// #ifndef UNICODE
// #define UNICODE
// #endif 

// #include <windows.h>
// #include <gdiplus.h>
// #include <vector>
// #include <string>
// using namespace Gdiplus;

// #pragma comment (lib, "gdiplus.lib")

// ULONG_PTR gdiplusToken;

// void Example_SetPixel(HDC hdc, int width, int height)
// {
//     std::vector<std::string> a;
//     for (int i = 0;i<1000;i++){
//         a.push_back("hello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello world");
//     }
//     Graphics graphics(hdc);

//     Bitmap myBitmap(width, height, PixelFormat32bppARGB);

//     // Create a checkered pattern with colorful pixels.
//     for (UINT y = 0; y < height; y += 2)
//     {
//         for (UINT x = 0; x < width; x += 1)
//         {
//             if (y/2 < a.size() && x < a[y/2].size() && a[y/2][x] != ' '){
//                 myBitmap.SetPixel(x, y, Color(255,255, 255, 255));
//                 myBitmap.SetPixel(x, y+1, Color(255,0,0, 0));
//             }
//             else{
//                 myBitmap.SetPixel(x, y, Color(255,0,0, 0));
//                 myBitmap.SetPixel(x, y+1, Color(255,0,0, 0));
//             }
//         }
//     }

//     // Draw the altered bitmap.
//     graphics.DrawImage(&myBitmap, 0, 0);
// }

// LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
// {
//     switch (msg)
//     {
//     case WM_PAINT:
//     {
//         PAINTSTRUCT ps;
//         HDC hdc = BeginPaint(hWnd, &ps);

//         Example_SetPixel(hdc, ps.rcPaint.right, ps.rcPaint.bottom);
//         EndPaint(hWnd, &ps);
//         return 0;
//     }
//     case WM_DESTROY:
//         PostQuitMessage(0);
//         break;
//     }

//     return DefWindowProc(hWnd, msg, wp, lp);
// }

// int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int cmdshow)
// {
//     GdiplusStartupInput gdiplusStartupInput;
//     GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

//     const wchar_t CLASS_NAME[] = L"TextEditor";

//     WNDCLASS wc = {};
//     wc.lpfnWndProc = WindowProc;
//     wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
//     wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//     wc.hInstance = hInst;
//     wc.lpszClassName = CLASS_NAME;
//     wc.style = CS_HREDRAW | CS_VREDRAW;

//     RegisterClass(&wc);

//     HWND hWnd = CreateWindowEx(
//         0,
//         CLASS_NAME,
//         L"CleanEdit",
//         WS_OVERLAPPEDWINDOW,
//         100, 100, 200, 500,
//         NULL,
//         NULL,
//         hInst,
//         NULL
//     );

//     ShowWindow(hWnd, cmdshow);

//     MSG msg = {};
//     while (GetMessage(&msg, NULL, 0, 0) > 0)
//     {
//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     }

//     // ⭐ Shutdown GDI+ ⭐
//     GdiplusShutdown(gdiplusToken);

//     return 0;
// }






// #include <iostream>
// #include <filesystem>

// using namespace std;
// namespace fs = std::filesystem;

//  int main() {
//         std::string path_to_directory = "E:/"; // Replace with your directory path

//         // Check if the path exists and is a directory
//         if (!fs::exists(path_to_directory) || !fs::is_directory(path_to_directory)) {
//             std::cerr << "Error: Invalid directory path." << std::endl;
//             return 1;
//         }

//         std::cout << "Listing contents of: " << path_to_directory << std::endl;

//         // Iterate over each entry in the directory
//         for (const auto& entry : fs::directory_iterator(path_to_directory)) {
//             // Get the path of the current entry
//             std::cout << entry.path().filename().string(); // Print only the file/folder name

//             // Check if it's a directory
//             if (fs::is_directory(entry.status())) {
//                 std::cout << " (Directory)";
//             } else if (fs::is_regular_file(entry.status())) {
//                 std::cout << " (File)";
//             }
//             std::cout << std::endl;
//         }

//         return 0;
//     }




// #ifndef UNICODE
// #define UNICODE
// #endif 

// #include "file_explorer.h"




// LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
// {
//     switch (msg)
//     {

//     case WM_PAINT:
//     {
//         PAINTSTRUCT ps;
//         HDC hdc = BeginPaint(hWnd, &ps);
//         createFileExplorer(hWnd,0,0,200,200);

//         EndPaint(hWnd, &ps);
//         return 0;
//     }
//     case WM_DESTROY:
//         PostQuitMessage(0);
//         break;
//     }

//     return DefWindowProc(hWnd, msg, wp, lp);
// }

// int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int cmdshow)
// {
//     const wchar_t CLASS_NAME[] = L"TextEditor";

//     WNDCLASS wc = {};
//     wc.lpfnWndProc = WindowProc;
//     wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
//     wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//     wc.hInstance = hInst;
//     wc.lpszClassName = CLASS_NAME;
//     wc.style = CS_HREDRAW | CS_VREDRAW;

//     RegisterClass(&wc);

//     HWND hWnd = CreateWindowEx(
//         0,
//         CLASS_NAME,
//         L"CleanEdit",
//         WS_OVERLAPPEDWINDOW,
//         100, 100, 500, 600,
//         NULL,
//         NULL,
//         hInst,
//         NULL
//     );

//     ShowWindow(hWnd, cmdshow);

//     MSG msg = {};
//     while (GetMessage(&msg, NULL, 0, 0) > 0)
//     {
//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     }

//     return 0;
// }



// #include <winsock2.h>
// #include <ws2tcpip.h>
// #include <windows.h>
// #include <iostream>

// #pragma comment(lib, "ws2_32.lib")

// #define PORT 8080

// int main()
// {
//     // Initialize Winsock
//     WSADATA wsaData;
//     int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);
//     if (wsaerr != 0) {
//         std::cerr << "WSAStartup failed with error: " << wsaerr << std::endl;
//         return 1;
//     }

//     // Create socket
//     SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == INVALID_SOCKET) {
//         std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
//         WSACleanup();
//         return 1;
//     }

//     // Specify address
//     sockaddr_in serverAddress{};
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_port = htons(PORT);
//     serverAddress.sin_addr.s_addr = INADDR_ANY;

//     // Bind
//     if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
//         std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
//         closesocket(serverSocket);
//         WSACleanup();
//         return 1;
//     }

//     // Listen
//     if (listen(serverSocket, 5) == SOCKET_ERROR) {
//         std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
//         closesocket(serverSocket);
//         WSACleanup();
//         return 1;
//     }

//     std::cout << "Server listening on port " << PORT << "...\n";

//     while (true) {
//         SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
//         if (clientSocket == INVALID_SOCKET) {
//             std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
//             continue;
//         }

//         char buffer[1024] = {0};
//         int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
//         if (bytesReceived > 0) {
//             buffer[bytesReceived] = '\0';
//             std::cout << "Message from client: " << buffer << std::endl;
//         } else {
//             std::cerr << "Receive failed or client disconnected.\n";
//         }

//         closesocket(clientSocket);
//     }

//     // Cleanup (this line never hits in an infinite loop, just for reference)
//     closesocket(serverSocket);
//     WSACleanup();
//     return 0;
// }







// #include <winsock2.h>
// #include <ws2tcpip.h>
// #include <windows.h>
// #include <iostream>
// #include <string>

// #pragma comment(lib, "ws2_32.lib")  // Link Winsock library

// #define PORT 8080
// #define SA struct sockaddr 



// void func(int sockfd)
// {
//     char buff[1024];
//     int n;
//     for (;;) {
//         printf("Enter the string : ");
//         n = 0;
//         while ((buff[n++] = getchar()) != '\n')
//             ;
//         send(sockfd, buff, sizeof(buff),0);
//         recv(sockfd, buff, sizeof(buff),0);
//         printf("From Server : %s", buff);
//         if ((strncmp(buff, "exit", 4)) == 0) {
//             printf("Client Exit...\n");
//             break;
//         }
//     }
// }

// int main()
// {
//     int sockfd, connfd;
//     struct sockaddr_in servaddr, cli;

//     // socket create and verification
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd == -1) {
//         printf("socket creation failed...\n");
//         exit(0);
//     }
//     else
//         printf("Socket successfully created..\n");

//     // assign IP, PORT
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//     servaddr.sin_port = htons(PORT);

//     // connect the client socket to server socket
//     if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
//         != 0) {
//         printf("connection with the server failed...\n");
//         exit(0);
//     }
//     else
//         printf("connected to the server..\n");

//     // function for chat
//     func(sockfd);
// }




// #include <winsock2.h>
// #include <ws2tcpip.h>
// #include <windows.h>

// #include <cstring>
// #include <iostream>

// #define PORT 8080

// int main() {
//   int sock = 0;
//   struct sockaddr_in serv_addr;
//   const char *hello = "Hello from client";

//   // Create socket
//   sock = socket(AF_INET, SOCK_STREAM, 0);
//   if (sock < 0) {
//     std::cerr << "Socket creation error" << std::endl;
//     return -1;
//   }

//   serv_addr.sin_family = AF_INET;
//   serv_addr.sin_port = htons(PORT);

//   // Convert IPv4 and IPv6 addresses from text to binary
//   if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
//     std::cerr << "Invalid address/ Address not supported" << std::endl;
//     return -1;
//   }

//   // Connect to server
//   if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
//     std::cerr << "Connection Failed" << std::endl;
//     return -1;
//   }

//   // Send data
//   send(sock, hello, strlen(hello), 0);
//   std::cout << "Message sent" << std::endl;

//   // Close socket
//   return 0;
// }



#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

std::string C_Program(std::string file_name,std::string file_path,std::vector<std::string> linkers){
            // g++ your_file.cpp -o output_name && ./output_name
           std::filesystem::path file(file_path);
            std::string command = "cd /" + file.parent_path().string() + " g++ " + file_name;
            int index = 0;
            for (int i = file_name.size()-1;i>=0;i--) if (file_name[i] == '.') {index = i; break;}
            // std::string out_name = file_name.substr(0,index);
            for (int i = 0;i<linkers.size();i++) command += " -"+ linkers[i];
            
            command += " -o " + file.filename().string() + " && ./" + file.filename().string();
            return command;
  }

int main(){
  std::cout << C_Program("hello_world.cpp","C:/hello_world.cpp",{});
}