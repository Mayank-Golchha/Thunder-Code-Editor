// #include <iostream>
// #include <string>
// // #include "text_plugin.h"


//  void tokenize(std::string code){
//             //will split code
//     int len = code.size();
//     int line_no = 0,start_index = 0,last = 0;
//     for (int i = 0;i<len;i++){
//         if (code[i] == ' ' || code[i] == '\n'){
//             if (i > last){
//                 std::string word = code.substr(last,i-last);
//                 std::cout << "word : " << word << " line no : " << line_no << " start : " << start_index - (i-last) << " end : " << start_index << "\n";
//             }
//             last = i+1;

//         }
//         start_index++;
//         if (code[i] == '\n'){
//             line_no++;
//             start_index = 0;
//         }
//     }   
// }


// int main(){
//     std::string code = " \n";
//     tokenize(code);
    
// }






#include <windows.h>
#include <winhttp.h>
#include <iostream>

int main() {
    // Step 1: Open a session
    HINTERNET hSession = WinHttpOpen(L"My User Agent/1.0",
                                     WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     WINHTTP_NO_PROXY_NAME,
                                     WINHTTP_NO_PROXY_BYPASS, 0);

    if (!hSession) {
        std::wcerr << L"Failed to open session\n";
        return 1;
    }

    // Step 2: Connect to the server
    HINTERNET hConnect = WinHttpConnect(hSession, L"www.example.com", INTERNET_DEFAULT_HTTP_PORT, 0);
    if (!hConnect) {
        std::wcerr << L"Failed to connect\n";
        WinHttpCloseHandle(hSession);
        return 1;
    }

    // Step 3: Create the request
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/",
                                            NULL, WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES,
                                            0);

    if (!hRequest) {
        std::wcerr << L"Failed to open request\n";
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return 1;
    }

    // Step 4: Send the request
    if (!WinHttpSendRequest(hRequest,
                            WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                            WINHTTP_NO_REQUEST_DATA, 0,
                            0, 0)) {
        std::wcerr << L"Failed to send request\n";
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return 1;
    }

    // Step 5: Receive the response
    if (!WinHttpReceiveResponse(hRequest, NULL)) {
        std::wcerr << L"Failed to receive response\n";
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return 1;
    }

    // Step 6: Read the response
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL bResults = TRUE;

    do {
        dwSize = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
            std::wcerr << L"Error querying data\n";
            break;
        }

        pszOutBuffer = new char[dwSize + 1];
        ZeroMemory(pszOutBuffer, dwSize + 1);

        if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
                             dwSize, &dwDownloaded)) {
            std::wcerr << L"Error reading data\n";
            delete[] pszOutBuffer;
            break;
        }

        std::cout << pszOutBuffer; // Print to console
        delete[] pszOutBuffer;

    } while (dwSize > 0);

    // Cleanup
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return 0;
}
