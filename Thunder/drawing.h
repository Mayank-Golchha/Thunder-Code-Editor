#pragma comment(lib, "msimg32.lib")

#ifndef DRAWING_H
#define DRAWING_H

#include <windows.h>
#include <wingdi.h>


void DrawLine(HDC hdc,int x1,int y1,int x2,int y2,int width,COLORREF color){
    HPEN hPen = CreatePen(PS_SOLID, width, color); // Red, 2px wide
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void DrawGradientRect(HDC hdc, RECT r, COLORREF topleftColor, COLORREF toprightColor,COLORREF bottomleftColor,COLORREF bottomrightColor) {
    TRIVERTEX vertex[4] = {
            {r.left, r.top, (COLOR16)(GetRValue(topleftColor) << 8), (COLOR16)(GetGValue(topleftColor) << 8), (COLOR16)(GetBValue(topleftColor) << 8), 0x0000},    // Top-left (blue)
            {r.right, r.top,(COLOR16)(GetRValue(toprightColor) << 8),(COLOR16)(GetGValue(toprightColor) << 8), (COLOR16)(GetBValue(toprightColor) << 8), 0x0000},    // Top-right (red)
            {r.left, r.bottom,(COLOR16)(GetRValue(bottomleftColor) << 8), (COLOR16)(GetGValue(bottomleftColor) << 8), (COLOR16)(GetBValue(bottomleftColor) << 8), 0x0000},    // Bottom-left (green)
            {r.right, r.bottom,(COLOR16)(GetRValue(bottomrightColor) << 8), (COLOR16)(GetGValue(bottomrightColor) << 8), (COLOR16)(GetBValue(bottomrightColor) << 8), 0x0000}     // Bottom-right (yellow)
        };

    GRADIENT_TRIANGLE triangles[2] = {
        {0, 1, 2}, // Top-left, top-right, bottom-left
        {2, 1, 3}  // Bottom-left, top-right, bottom-right
    };

        // Draw the gradient
    GradientFill(hdc, vertex, 4, triangles, 2, GRADIENT_FILL_TRIANGLE);
}

void DrawGradientLine(HDC hdc, int x1, int x2, int y, COLORREF startColor, COLORREF endColor) {
    int width = x2 - x1;

    BYTE r1 = GetRValue(startColor);
    BYTE g1 = GetGValue(startColor);
    BYTE b1 = GetBValue(startColor);

    BYTE r2 = GetRValue(endColor);
    BYTE g2 = GetGValue(endColor);
    BYTE b2 = GetBValue(endColor);

    for (int i = 0; i < width; ++i) {
        float t = (float)i / width;

        BYTE r = (BYTE)(r1 + t * (r2 - r1));
        BYTE g = (BYTE)(g1 + t * (g2 - g1));
        BYTE b = (BYTE)(b1 + t * (b2 - b1));

        SetPixel(hdc, x1 + i, y, RGB(r, g, b));
        SetPixel(hdc, x1 + i, y + 1, RGB(r, g, b));
    }
}

// void DrawTransparentRect(HDC hdcDest, int x, int y, int width, int height, COLORREF color, BYTE alpha) {
//     // Create a memory DC and a 32-bit bitmap for transparency
//     HDC hdcMem = CreateCompatibleDC(hdcDest);

//     BITMAPINFO bmi = { 0 };
//     bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//     bmi.bmiHeader.biWidth = width;
//     bmi.bmiHeader.biHeight = -height;  // top-down DIB
//     bmi.bmiHeader.biPlanes = 1;
//     bmi.bmiHeader.biBitCount = 32;     // 32-bit bitmap for alpha
//     bmi.bmiHeader.biCompression = BI_RGB;

//     void* pvBits;
//     HBITMAP hBmp = CreateDIBSection(hdcDest, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
//     HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hBmp);

//     // Fill the memory bitmap with desired color and alpha
//     DWORD* pixels = (DWORD*)pvBits;
//     BYTE r = GetRValue(color);
//     BYTE g = GetGValue(color);
//     BYTE b = GetBValue(color);

//     for (int i = 0; i < width * height; ++i) {
//         pixels[i] = (alpha << 24) | (r << 16) | (g << 8) | b;
//     }

//     // Setup blend function
//     BLENDFUNCTION bf = { 0 };
//     bf.BlendOp = AC_SRC_OVER;
//     bf.BlendFlags = 0;
//     bf.SourceConstantAlpha = 255;         // Use per-pixel alpha
//     bf.AlphaFormat = AC_SRC_ALPHA;        // Use alpha channel from bitmap

//     AlphaBlend(hdcDest, x, y, width, height, hdcMem, 0, 0, width, height, bf);

//     // Cleanup
//     SelectObject(hdcMem, hOldBmp);
//     DeleteObject(hBmp);
//     DeleteDC(hdcMem);
// }


void DrawTransparentRect(HDC hdcDest, RECT rect, COLORREF color, BYTE alpha) {
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Create a memory DC and 32-bit bitmap for transparency
    HDC hdcMem = CreateCompatibleDC(hdcDest);

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* pvBits;
    HBITMAP hBmp = CreateDIBSection(hdcDest, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hBmp);

    // Fill memory bitmap with desired color and alpha
    DWORD* pixels = (DWORD*)pvBits;
    BYTE r = GetRValue(color);
    BYTE g = GetGValue(color);
    BYTE b = GetBValue(color);

    for (int i = 0; i < width * height; ++i) {
        pixels[i] = (alpha << 24) | (r << 16) | (g << 8) | b;
    }

    // Setup blend function
    BLENDFUNCTION bf = { 0 };
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 255;     // Use per-pixel alpha
    bf.AlphaFormat = AC_SRC_ALPHA;

    AlphaBlend(hdcDest, rect.left, rect.top, width, height,
               hdcMem, 0, 0, width, height, bf);

    // Cleanup
    SelectObject(hdcMem, hOldBmp);
    DeleteObject(hBmp);
    DeleteDC(hdcMem);
}




#endif