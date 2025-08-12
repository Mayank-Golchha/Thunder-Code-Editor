
// #include "c_parser.h"
// #include "c_plugin.h"

// #include <iostream>

// int main(){
//     std::string code = "{int a = 2} int c = 3;int b";
//     CPlugin plugin;
//     // plugin.init(code);
//     std::vector<C_Token*> tokens = plugin.load(code,1,0);
//     C_Parser parser(tokens,NULL,0,0);
//     parser.parse();
//     std::cout << "done\n";
//     std::cout << parser.current_block->declaration_statements.size();
//     // for (int i = 0;i<parser.current_block->declaration_statements.size();i++){
//     // for (auto i : parser.current_block->declaration_statements){
//     for (auto i : parser.global_block->declaration_statements){
//             std::cout << i->name << "\n";
//     }

//     std::cout << "\nreached";

//     // plugin.display(0,0,0,0);
//     // for (int i = 0;i<plugin.style_tokens.size();i++){
//         // std::cout << "tok : " << plugin.style_tokens[i]->start << " " << plugin.style_tokens[i]->end << " " <<  plugin.style_tokens[i]->startLine << " " << plugin.style_tokens[i]->endLine << "\n";
//     // }

// }










// #include <winsock2.h>
// #include <ws2tcpip.h>
// #include <iostream>
// #include <string>
// // #include "./JsonParser/parser.h"
// #include <regex>
// #include "E:/Text Editor/JsonParser/parser.h"

// #pragma comment(lib, "Ws2_32.lib")

// int main() {
//     WSADATA wsaData;
//     SOCKET sock = INVALID_SOCKET;
//     struct sockaddr_in server;
//     const char* serverName = "127.0.0.1";
//     int port = 11434;

//     // Initialize Winsock
//     if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
//         std::cerr << "WSAStartup failed.\n";
//         return 1;
//     }

//     // Create socket
//     sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//     if (sock == INVALID_SOCKET) {
//         std::cerr << "Socket creation failed.\n";
//         WSACleanup();
//         return 1;
//     }

//     // Set up server struct
//     server.sin_family = AF_INET;
//     server.sin_port = htons(port);
//     inet_pton(AF_INET, serverName, &server.sin_addr);

//     // Connect to server
//     if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
//         std::cerr << "Connection failed.\n";
//         closesocket(sock);
//         WSACleanup();
//         return 1;
//     }


//     std::string prompt = 
//     "Continue the following C++ code and generate ONLY the next token.\\n"
//     "Return only the next word.\\n\\n"
//     "Code:\\n"
//     "std::cout << \\\"Hello W\\n\\n"
//     "Next token:";

//     std::string json = R"({
//     "model": "gemma3",
//     "prompt": ")" + prompt + R"(",
//     "stream": false
// })";


//     // Prepare HTTP request
//     std::string request = 
//         "POST /api/generate HTTP/1.1\r\n"
//         "Host: localhost:11434\r\n"
//         "Content-Type: application/json\r\n"
//         "Content-Length: " + std::to_string(json.length()) + "\r\n"
//         "Connection: close\r\n"
//         "\r\n" + json;

//     // Send request
//     send(sock, request.c_str(), request.size(), 0);

//     // Receive response
//     char buffer[4096];
//     int bytesReceived;
//     std::string response;
//     while ((bytesReceived = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
//         response.append(buffer, bytesReceived);
//     }

//     std::smatch match;

//     std::regex re("\"response\"\\s*:\\s*\"([^\"]*)\"");



//     if (std::regex_search(response, match, re)) {
//         std::string responseValue = match[1].str();
//         size_t pos;
//         std::cout << "response: " << responseValue << "\n";
//     } else {
//         std::cerr << "No response field found.\n";
//     }



//     // std::cout << int('"');
//     // std::cout << "Response:\n" << std::string(n["model"]) << std::endl;

//     // Clean up
//     closesocket(sock);
//     WSACleanup();
//     return 0;
// }




// cd "e:\Text Editor\Plugins\C_Plugin\" ; if ($?) { g++ m.cpp -lws2_32 -o m } ; if ($?) { .\m }


#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <regex>
#include <locale>
#include <codecvt>
#include <sstream>

// #include "../JsonParser/parser.h"
#include "../../JsonParser/parser.h"

#pragma comment(lib, "Ws2_32.lib")

std::string loadFile(const std::string& path) {
    std::ifstream file(path);
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::string buildGemmaPrompt(const std::string& apiDescription, const std::string& taskDescription) {
    return apiDescription + "\n\n" + "## Task:\n" + taskDescription;
}

std::string escapeJson(const std::string& input) {
    std::ostringstream ss;
    for (char c : input) {
        switch (c) {
            case '\"': ss << "\\\""; break;
            case '\\': ss << "\\\\"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default: ss << c;
        }
    }
    return ss.str();
}

std::string getTokenFromOllama(const std::string& codeFragment) {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in server;
    const char* serverName = "127.0.0.1";
    int port = 11434;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return "WSAStartup failed.";

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return "Socket creation failed.";
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, serverName, &server.sin_addr);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        return "Connection failed.";
    }

    // std::string prompt = "Continue the following C++ code and generate ONLY the next token.\\n"
                        //  "Return only the next word.\\n\\n"
                        //  "Code:\\n" + codeFragment + "\\n\\nNext token:";

    // std::string prompt = 
        // "Continue the following C++ code and generate ONLY the next token.\\n"
        // "Return only the next word.\\n"
        // "Do NOT use Unicode escapes like \\u003c — use real characters like <, >, etc.\\n\\n"
        // "Code:\\n" + codeFragment + "\\n\\nNext token:";

    std::string apiText = loadFile("temp.txt");

    std::string task = "Write a new extension that inserts a semicolon ';' automatically when the user types Enter after a line ending with a statement.";

    std::string prompt = buildGemmaPrompt(apiText, task);


    prompt = escapeJson(prompt);
    // std::cout << prompt;


    std::string json = R"({"model": "gemma3","prompt": ")" + prompt + R"(","stream": false})";

    std::string request =
        "POST /api/generate HTTP/1.1\r\n"
        "Host: localhost:11434\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: " + std::to_string(json.length()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" + json;

    send(sock, request.c_str(), request.size(), 0);

    char buffer[4096];
    int bytesReceived;
    std::string response;
    while ((bytesReceived = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, bytesReceived);
    }

    closesocket(sock);
    WSACleanup();
    std::cout << "\n\n\n\n\n RESPONSE \n\n\n\n" << response << "\n";

    Json json_(response);
    JsonNode node =  *json_.parse();

    std::string out = std::string(node["response"]);
    return out;


    // std::string c = Json()

    // std::smatch match;
    // std::regex re("\"response\"\\s*:\\s*\"([^\"]*)\"");

    // if (std::regex_search(response, match, re)) {
        // std::string token = match[1].str();
        // size_t pos;
        // while ((pos = token.find("\\n")) != std::string::npos)
            // token.replace(pos, 2, "\n");

        // return token;
    // } else {
        // return "No token found.";
    // }
}

int main() {
    std::string code;
    std::cout << "Type code fragment and press Enter (type 'exit' to quit):\n";

    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, code);

        if (code == "exit" || code == "q") break;

        std::string result = getTokenFromOllama(code);

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

// Convert UTF-8 to wide string (UTF-16)
std::wstring wide_string = converter.from_bytes(result); 
        std::wcout << wide_string << std::endl;
    }

    return 0;
}
