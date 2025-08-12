//to open file and folder

#ifndef OPEN_FILE_H
#define OPEN_FILE_H


#include <windows.h>
#include <string>
#include <shlobj.h>

std::string open_file(HWND hWnd) {
    OPENFILENAME ofn;
    TCHAR file_name[MAX_PATH] = {0};  
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = file_name;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"C++ Files\0*.CPP\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (!GetOpenFileName(&ofn)) {
        return ""; 
    }

    std::wstring ws(file_name);
    std::string file_path_(ws.begin(), ws.end());

    char name[100];
    int len = file_path_.length();
    int i = len - 1, index = 0;
    while (i >= 0 && file_path_[i] != '\\') i--;
    for (int j = i + 1; j < len; j++) name[index++] = file_path_[j];
    name[index] = '\0';

    return file_path_;
}


std::wstring openFolder(HWND hWnd){
     std::wstring folderPath;

    IFileDialog* pfd = nullptr;

    // Initialize COM
    HRESULT hr = CoInitialize(NULL);
    if (SUCCEEDED(hr)) {
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
                              IID_PPV_ARGS(&pfd));
        if (SUCCEEDED(hr)) {
            DWORD options;
            pfd->GetOptions(&options);
            pfd->SetOptions(options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);

            hr = pfd->Show(hWnd);
            if (SUCCEEDED(hr)) {
                IShellItem* psi;
                hr = pfd->GetResult(&psi);
                if (SUCCEEDED(hr)) {
                    PWSTR path = NULL;
                    psi->GetDisplayName(SIGDN_FILESYSPATH, &path);
                    folderPath = path;
                    CoTaskMemFree(path);
                    psi->Release();
                }
            }
            pfd->Release();
        }
        CoUninitialize();
    }

    return folderPath;
}
#endif