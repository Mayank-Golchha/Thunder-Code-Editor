// cd "d:\C Projects\c++\Text Editor\" ; if ($?) { g++ main.cpp -lgdi32 -o main } ; if ($?) { .\main }
// cd "e:\Text Editor\" ; if ($?) { g++ main.cpp -lgdi32 -lmsimg32 -ldwmapi -o main } ; if ($?) { .\main }
//  cd "e:\Text Editor\" ; if ($?) { g++ main.cpp -lgdi32 -lmsimg32 -ldwmapi -lgdiplus -o main } ; if ($?) { .\main }
#ifndef UNICODE
#define UNICODE
#endif 
#include "windowprocedure.h"

#pragma comment (lib, "gdiplus.lib")

ULONG_PTR gdiplusToken;

void RegisterMenuContainerWindow(){
    //menu bar
    WNDCLASS wc = {0};
    wc.lpfnWndProc = MenuContainerProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"MenuContainerClass";
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);

    RegisterClass(&wc);
}

void RegisterMenuWindow(){
    WNDCLASS wc = {0};
    wc.lpfnWndProc = MenuContainerProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"MenuWindowClass";
    wc.hCursor = LoadCursor(NULL,IDC_HAND);

    RegisterClass(&wc);
}


void RegisterFileTabContainerWindow(){
    WNDCLASS wc = {0};
    wc.lpfnWndProc = FileTabContainerProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"FileTabContainerClass";
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);

    RegisterClass(&wc);
}

void RegisterCustomEditWindow(){
    WNDCLASS wc = {0};
    wc.lpfnWndProc = CustomEditWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"CustomEditWindowClass";
    wc.style = CS_DBLCLKS;
    wc.hCursor = LoadCursor(NULL,IDC_IBEAM);
    RegisterClass(&wc);
}

void RegisterAutoCompleteWindow(){
    WNDCLASS wc = {0};
    wc.lpfnWndProc = autoCompleteWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"AutoCompleteWindow";
    wc.hCursor = LoadCursor(NULL,IDC_HAND);
    RegisterClass(&wc);
}

void RegisterFileExplorerWindow(){
    WNDCLASS wc = {0};
    wc.lpfnWndProc = FileExplorerProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"FileExplorerWindowClass";
    // wc.style = CS_DBLCLKS;
    wc.hCursor = LoadCursor(NULL,IDC_HAND);
    RegisterClass(&wc);
}


void RegisterInputWindow(){
    WNDCLASS wc = {0};
    wc.lpfnWndProc = InputWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"InputWindowClass";
    wc.hCursor = LoadCursor(NULL,IDC_IBEAM);
    RegisterClass(&wc);
}


void RegisterContextMenuWindow(){
    WNDCLASS wc = {0};
    wc.lpfnWndProc = ContextMenuWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"ContextMenuWindowClass";
    wc.hCursor = LoadCursor(NULL,IDC_HAND);
    RegisterClass(&wc);
}


void RegisterCustomStaticWindow(){
    WNDCLASS wc = {0};
    wc.lpfnWndProc = CustomWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"CustomStaticWindow";
    wc.hCursor = LoadCursor(NULL,IDC_HAND);
    RegisterClass(&wc);
}



// void RegisterMiniCodeViewerWindow(){
    // WNDCLASS wc = {};
    // wc.lpfnWndProc = MiniCodeViewerProc;
    // wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    // wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    // wc.lpszClassName = L"MiniCodeViewerWindow";
    // wc.style = CS_HREDRAW | CS_VREDRAW;
    // RegisterClass(&wc);
// }


int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR args,int cmdshow){
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    
    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"TextEditor";
    
    WNDCLASS wc = {};

    wc.lpfnWndProc   = WindowProc;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hInstance     = hInst;
    wc.lpszClassName = CLASS_NAME;

    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    RegisterMenuContainerWindow();
    RegisterFileTabContainerWindow();
    RegisterCustomEditWindow();
    RegisterAutoCompleteWindow();
    RegisterMenuWindow();
    RegisterFileExplorerWindow();
    RegisterInputWindow();
    RegisterContextMenuWindow();

    RegisterCustomStaticWindow();
    // RegisterMiniCodeViewerWindow();

    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

    // Create the window.
    hmainWindow = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"CleanEdit",    // Window text
        WS_POPUP | WS_THICKFRAME | WS_CLIPCHILDREN, // WS_THICKFRAME enables resizing
        // WS_POPUP | WS_VISIBLE, // WS_THICKFRAME enables resizing

        // WINDOW_X, WINDOW_Y, WIDTH, HEIGHT,
        // 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        workArea.left,workArea.top,workArea.right,workArea.bottom,
        NULL,       // Parent window    
        NULL,       // Menu
        hInst,  // Instance handle
        NULL        // Additional application data
    );

    if (!hmainWindow) return 0;

    ShowWindow(hmainWindow, cmdshow);

    // LoadExtension(L"Extensions/hello_world.dll", &editorApi);
    LoadExtension(L"Extensions/auto_pair.dll", &editorApi);
    LoadExtension(L"Extensions/CFunctionDoc.dll", &editorApi);
    LoadExtension(L"Extensions/html_tag_completer.dll", &editorApi);


    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);
    return 0;
}

