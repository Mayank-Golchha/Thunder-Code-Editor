// #include "json_plugin.h"
// #include <iostream>

// int main(){
//     // std::string code = "\"1a\n1";
//     std::string code = "asdasf\nas";
//     JsonPlugin plugin;
//     plugin.load(code,0,0);
//     for (int i = 0;i<plugin.style_tokens.size();i++){
//         std::cout << plugin.style_tokens[i]->startLine << " " << plugin.style_tokens[i]->endLine << "\n";
//     }
//     // std::cout << plugin.style_tokens[0]->start << "  " << plugin.style_tokens[0]->end << "\n";
//     // std::cout << plugin.style_tokens[1]->start << "  " << plugin.style_tokens[1]->end << "\n";
//     // std::cout << plugin.style_tokens[2]->start << "  " << plugin.style_tokens[2]->end << "\n";
//     // std::cout << plugin.style_tokens[3]->start << "  " << plugin.style_tokens[3]->endLine << "\n";
// }


#include <windows.h>
#include <iostream>
#include <string>

#ifndef PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE
#define PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE 0x00020016
#endif

typedef HANDLE HPCON;
typedef HRESULT (WINAPI *CreatePseudoConsole_t)(COORD, HANDLE, HANDLE, DWORD, HPCON*);
typedef void (WINAPI *ClosePseudoConsole_t)(HPCON);

int main() {
    HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
    CreatePseudoConsole_t CreatePseudoConsole = (CreatePseudoConsole_t)GetProcAddress(hKernel32, "CreatePseudoConsole");
    ClosePseudoConsole_t ClosePseudoConsole = (ClosePseudoConsole_t)GetProcAddress(hKernel32, "ClosePseudoConsole");

    if (!CreatePseudoConsole || !ClosePseudoConsole) {
        std::cout << "❌ ConPTY not supported.\n";
        return 1;
    }

    // 1. Create pipes
    HANDLE hPipeInRead = NULL, hPipeInWrite = NULL;
    HANDLE hPipeOutRead = NULL, hPipeOutWrite = NULL;
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
    CreatePipe(&hPipeInRead, &hPipeInWrite, &sa, 0);
    CreatePipe(&hPipeOutRead, &hPipeOutWrite, &sa, 0);

    // 2. Create Pseudo Console
    HPCON hPC;
    COORD size = { 80, 25 };
    HRESULT hr = CreatePseudoConsole(size, hPipeInRead, hPipeOutWrite, 0, &hPC);
    if (FAILED(hr)) {
        std::cout << "❌ Failed to create PseudoConsole.\n";
        return 1;
    }

    // 3. Setup Startup Info
    STARTUPINFOEXW si = { 0 };
    si.StartupInfo.cb = sizeof(STARTUPINFOEXW);

    SIZE_T attrListSize = 0;
    InitializeProcThreadAttributeList(NULL, 1, 0, &attrListSize);
    si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, attrListSize);
    InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &attrListSize);
    UpdateProcThreadAttribute(si.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE, hPC, sizeof(hPC), NULL, NULL);

    // 4. Launch cmd.exe
    PROCESS_INFORMATION pi = {};
    wchar_t cmd[] = L"cmd.exe";
    BOOL success = CreateProcessW(NULL, cmd, NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT,
                                  NULL, NULL, &si.StartupInfo, &pi);
    if (!success) {
        std::cout << "❌ Failed to start cmd.exe\n";
        return 1;
    }

    std::cout << "✅ cmd.exe launched with ConPTY.\n";

    // 5. Read terminal output (non-blocking for this demo)
    char buffer[256];
    DWORD bytesRead;
    while (ReadFile(hPipeOutRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        buffer[bytesRead] = 0;
        std::cout << buffer; // Replace this with your editor text rendering
    }

    // 6. Cleanup
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    ClosePseudoConsole(hPC);

    return 0;
}
