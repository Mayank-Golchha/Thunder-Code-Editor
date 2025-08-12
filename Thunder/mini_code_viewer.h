// #ifndef UNICODE
// #define UNICODE
// #endif 

#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>

// #include "editorFunctions.h"

// // using namespace Gdiplus;



// void draw_pixels(HDC hdc,int width, int height)
// {
//    //  std::vector<std::string> a;
//    //  for (int i = 0;i<1000;i++){
//       //   a.push_back("hello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello worldhello world hello world");
//    //  }
//    Gdiplus::Graphics graphics(hdc);

//    Gdiplus::Bitmap myBitmap(width, height, PixelFormat32bppARGB);

//     // Create a checkered pattern with colorful pixels.
//    for (UINT y = 0; y < height; y += 2){
//       for (UINT x = 0; x < width; x += 1){
//          if (y/2 < editContent.size() && x < editContent[y/2].size() && editContent[y/2][x] != ' '){
//             myBitmap.SetPixel(x, y, Gdiplus::Color(255,255, 255, 255));
//             myBitmap.SetPixel(x, y+1, Gdiplus::Color(255,50,100,30));
//          }
//          else{
//             myBitmap.SetPixel(x, y, Gdiplus::Color(255,50,100,30));
//             myBitmap.SetPixel(x, y+1, Gdiplus::Color(255,50,100,30));
//          }
//       }
//    }

//     // Draw the altered bitmap.
//    graphics.DrawImage(&myBitmap, 0, 0);
// }

// LRESULT CALLBACK MiniCodeViewerProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
//    static HDC memDC = nullptr;
//    static HBITMAP memBitmap = nullptr;

//    switch (msg){
//       case WM_CREATE:
//       {
//          HDC hdc = GetDC(hWnd);
//          RECT rect;
//          GetClientRect(hWnd, &rect);

//          memDC = CreateCompatibleDC(hdc);
//          memBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
//          SelectObject(memDC, memBitmap);

//          ReleaseDC(hWnd, hdc);
//          return 0;
//       }

//       case WM_PAINT:
//       {
//          PAINTSTRUCT ps;
//          HDC hdc = BeginPaint(hWnd, &ps);
//          // HDC memDC = CreateCompatibleDC(hdc);
//          // HBITMAP memBitmap = CreateCompatibleBitmap(hdc, editorWidth, editorHeight);
//          // HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

//          RECT rect;
//          GetClientRect(hWnd, &rect);

//         // Clear the buffer first (optional if you fully overwrite)
//          HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0)); // Background color
//          FillRect(memDC, &rect, brush);
//          DeleteObject(brush);

//         // Draw to memDC using GDI+
//          draw_pixels(memDC, rect.right, rect.bottom);

//         // Blit the buffer to the window
//          BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

//          EndPaint(hWnd, &ps);
//          return 0;
//       }

//       case WM_ERASEBKGND:
//          return 1;

//       case WM_DESTROY:
//          PostQuitMessage(0);
//          return 0;
//    }

//    return DefWindowProc(hWnd, msg, wp, lp);
// }

// HWND createMiniCodeViewer(HWND hWndparent,int x,int y,int width,int height){
//    HWND w = CreateWindowEx(0,L"MiniCodeViewerWindow",L"",WS_VISIBLE | WS_CHILD,
//       x, y, width, height,
//       hWndparent,NULL,NULL,NULL);

//    return w;
// }


COLORREF reverseColor(COLORREF color){
   BYTE r = GetRValue(color);
   BYTE g = GetGValue(color);
   BYTE b = GetBValue(color);
   return (r << 16) | (g << 8) | b;
}


std::vector<COLORREF> createMiniCodeViewer(HDC hdc,std::vector<std::wstring>* content,int start_line,int x_,int y_,int width,int height,COLORREF mini_code_viewer_bk){
   // x_,y_ are coordintes f top left corner of mini code viewer
   std::vector<COLORREF> pixels;
   COLORREF bkcolor = reverseColor(mini_code_viewer_bk);
   COLORREF txtcolor = reverseColor(colorTheme.miniCodeViewertc);
   pixels.resize(width*height,bkcolor);
   
   for (int y = 0;y+1<height;y+=2){
      for (int x = 0;x<width;x++){
         if (y/2 + start_line < content->size() && x < (*content)[y/2 + start_line].size() && (*content)[y/2 + start_line][x] != ' '){
            // pixels[y*width+x] = RGB(255,255,255);
            // pixels[y*width+x] = RGB(100,100,100);
            pixels[y*width+x] = txtcolor;
            // if (y + 1 < height)
            pixels[(y+1)*width + x] = bkcolor;

            // pixels[(y+1)*width+x] = mini_code_viewer_bk;
         }
      }
   }

   BITMAPINFO bmi = {};
      bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bmi.bmiHeader.biWidth = width;
      bmi.bmiHeader.biHeight = -height; // Negative for top-down DIB
      bmi.bmiHeader.biPlanes = 1;
      bmi.bmiHeader.biBitCount = 32;
      bmi.bmiHeader.biCompression = BI_RGB;

        // Draw pixel buffer to screen
   SetDIBitsToDevice(
      hdc, x_, y_, width, height, // draw rectangle at (50,50)
      0, 0, 0, height,
      pixels.data(), &bmi, DIB_RGB_COLORS
   );

   return pixels;
}