// #include "python_plugin.h"

// int main(){
//     // std::string code = "print(\"Hello World\")";
//     std::string code = "a \"asd\"";
//     PythonPlugin plugin;
//     plugin.init(code);
//     std::vector<Python_Token*> tokens = plugin.tokenize();
//     // std::cout << tokens[2]->raw_data << "g";
//     // std::cout << tokens[2]->raw_data << "g"; 
//     for (int i = 0;i<plugin.style_tokens.size();i++){
//         std::cout << plugin.style_tokens[i]->start << " " << plugin.style_tokens[i]->end << "\n";
//     }

// }

#include "python_parser.h"
#include "python_plugin.h"

// #include <iostream>

int main(){
    // std::string code = "a=((3)+2) b = a";
    // std::string code = "a = [1,(2+4)] b = [a]";
    // std::string code = "def hello(bo,cute):\n def he():\n  t = k()";
    // std::string code = "asd'hello\nworld' hi";
    // std::string code = "//             // Check if it's a directory\n   if (fs::is_directory(entry.status())) {\n  std::cout << \" (";
    // std::string code = "print(hello world)";
    std::string code = "import pygame \nclass";
    // std::string code = "'hello\nworld' hi";
    // std::string code = "hi";
    // std::string code = ":\n \n  \n ";
    // std::string code = "a = 1";
    PythonPlugin plugin;
    // plugin.init(code);
    std::vector<Python_Token*> tokens = plugin.load(code,1,0);
    // PythonParser parser(tokens);
    // parser.parse();
    std::cout << "done\n";
    plugin.display(0,0,0,0);
    for (int i = 0;i<plugin.style_tokens.size();i++){
        std::cout << "tok : " << plugin.style_tokens[i]->start << " " << plugin.style_tokens[i]->end << " " <<  plugin.style_tokens[i]->startLine << " " << plugin.style_tokens[i]->endLine << "\n";
    }
    // if (plugin.tokens.size()){
        // std::cout << plugin.tokens[1]->key;
    // }

    // std::cout << 
    // for (auto tok : parser.current_block->parent_block->parent_block->declaration_statements){
    
    // for (auto tok : parser.current_block->declaration_statements){
        // std::cout << tok->name;
        // for (auto k : tok->parameters){
            // std::cout << k;
        // }
    // }
    // std::cout << plugin.selected_token->raw_data;
    // std::cout << parser.indentation_stack->data;
    // std::cout << parser.indentation_stack->next->data;

    // std::cout << parser.varInBlock(parser.current_block->parent_block,"b");
    // std::cout << parser.varLookup(parser.current_block,"b");
    // std::cout << parser.varInBlock(parser.current_block->parent_block,"b");

}







// // // #include <winsock2.h>
// // // #include <ws2tcpip.h>
// // // #include <iostream>

// // // #pragma comment(lib, "ws2_32.lib")

// // // int main() {
// // //     WSADATA wsaData;
// // //     int iResult;

// // //     // Initialize Winsock
// // //     iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
// // //     if (iResult != 0) {
// // //         std::cerr << "WSAStartup failed: " << iResult << std::endl;
// // //         return 1;
// // //     }

// // //     addrinfo hints = {};
// // //     hints.ai_family = AF_UNSPEC;
// // //     hints.ai_socktype = SOCK_STREAM;
// // //     hints.ai_protocol = IPPROTO_TCP;

// // //     addrinfo* result = nullptr;
// // //     iResult = getaddrinfo("example.com", "80", &hints, &result); // Using HTTP port 80

// // //     if (iResult != 0) {
// // //         std::cerr << "getaddrinfo failed: " << iResult << std::endl;
// // //         WSACleanup();
// // //         return 1;
// // //     }

// // //     SOCKET ConnectSocket = INVALID_SOCKET;

// // //     for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
// // //         ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
// // //         if (ConnectSocket == INVALID_SOCKET) {
// // //             continue;
// // //         }

// // //         iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
// // //         if (iResult == SOCKET_ERROR) {
// // //             closesocket(ConnectSocket);
// // //             ConnectSocket = INVALID_SOCKET;
// // //             continue;
// // //         }
// // //         break;
// // //     }

// // //     freeaddrinfo(result);

// // //     if (ConnectSocket == INVALID_SOCKET) {
// // //         std::cerr << "Unable to connect to server!" << std::endl;
// // //         WSACleanup();
// // //         return 1;
// // //     }

// // //     // Send an HTTP GET request
// // //     const char* sendbuf = "GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n";
// // //     send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);

// // //     char recvbuf[4096];
// // //     int recvbuflen;

// // //     do {
// // //         recvbuflen = recv(ConnectSocket, recvbuf, sizeof(recvbuf) - 1, 0);
// // //         if (recvbuflen > 0) {
// // //             recvbuf[recvbuflen] = '\0'; // Null-terminate the received string
// // //             std::cout << recvbuf;
// // //         }
// // //     } while (recvbuflen > 0);

// // //     closesocket(ConnectSocket);
// // //     WSACleanup();
// // //     return 0;
// // // }





// // #include <winsock2.h>
// // #include <ws2tcpip.h>
// // #include <windows.h>
// // #include <unistd.h>

// // #include <cstring>
// // #include <iostream>

// // #define PORT 8080

// // int main()
// // {
// //     // creating socket
// //     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

// //     // specifying address
// //     sockaddr_in serverAddress;
// //     serverAddress.sin_family = AF_INET;
// //     serverAddress.sin_port = htons(80);
// //     serverAddress.sin_addr.s_addr = INADDR_ANY;

// //     // sending connection request
// //     connect(clientSocket, (struct sockaddr*)&serverAddress,
// //             sizeof(serverAddress));

// //     // sending data
// //     const char* message = "GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n";
// //     send(clientSocket, message, strlen(message), 0);

// //     // closing socket
// //     close(clientSocket);

// //     return 0;
// // }








// // #include <winsock2.h>
// // #include <ws2tcpip.h>
// // #include <windows.h>
// // #include <iostream>

// // #pragma comment(lib, "ws2_32.lib")  // Link Winsock library

// // #define PORT 8080

// // int main()
// // {
// //     // Initialize Winsock
// //     WSADATA wsaData;
// //     int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);
// //     if (wsaerr != 0) {
// //         std::cerr << "WSAStartup failed with error: " << wsaerr << std::endl;
// //         return 1;
// //     }

// //     while (true) {
// //         // Create socket
// //         SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
// //         if (clientSocket == INVALID_SOCKET) {
// //             std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
// //             break;
// //         }

// //         // Specify server address
// //         sockaddr_in serverAddress;
// //         serverAddress.sin_family = AF_INET;
// //         serverAddress.sin_port = htons(PORT);
// //         inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr); // Localhost

// //         // Connect to server
// //         if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
// //             std::cerr << "Connection failed. Error: " << WSAGetLastError() << std::endl;
// //             closesocket(clientSocket);
// //             break;
// //         }

// //         // Send data
// //         // const char* message;
// //         std::string message;
// //         std:: cin >> message;
        
// //         int sendResult = send(clientSocket, message.c_str(), strlen(message.c_str()), 0);
// //         if (message == ""){
// //         closesocket(clientSocket);
// //         continue;
// //         }
// //         if (sendResult == SOCKET_ERROR) {
// //             std::cerr << "Send failed. Error: " << WSAGetLastError() << std::endl;
// //             closesocket(clientSocket);
// //             break;
// //         }

// //         std::cout << "Message sent successfully.\n";

// //         // Close socket
// //         closesocket(clientSocket);

// //         Sleep(1000); // Wait 1 second before next send
// //     }

// //     // Cleanup
// //     WSACleanup();
// //     return 0;
// // }







// //server

// #include <winsock2.h>
// #include <ws2tcpip.h>
// #include <windows.h>
// #include <iostream>
// #include <string>

// #pragma comment(lib, "ws2_32.lib")  // Link Winsock library

// #define PORT 8080
// #define SA struct sockaddr 


// void func(int connfd) 
// { 
//     // char buff[MAX]; 
//     char buff[1024]; 
//     int n; 
//     // infinite loop for chat 
//     for (;;) { 
//         // read the message from client and copy it in buffer 
//         recv(connfd, buff, sizeof(buff)-1,0); 
//         // print buffer which contains the client contents 
//         std::cout << "From client: %s\t To client : " << buff; 
//         n = 0; 
//         // copy server message in the buffer 
//         while ((buff[n++] = getchar()) != '\n') 
//             ; 
  
//         // and send that buffer to client 
//         send(connfd, buff, sizeof(buff),0); 
  
//         // if msg contains "Exit" then server exit and chat ended. 
//         if (strncmp("exit", buff, 4) == 0) { 
//             printf("Server Exit...\n"); 
//             break; 
//         } 
//     } 
// } 

// int main(){
//     int sockfd, connfd, len; 
//     struct sockaddr_in servaddr, cli; 

//     sockfd = socket(AF_INET, SOCK_STREAM, 0); 

//     // if (sockfd == INVALID_SOCKET) { 
//         // printf("socket creation failed...\n"); 
//         // exit(0); 
//     // } 
//     // else
//         printf("Socket successfully created..\n"); 

//     // assign IP, PORT 
//     servaddr.sin_family = AF_INET; 
//     servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
//     servaddr.sin_port = htons(PORT); 

//     if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
//         printf("socket bind failed...\n"); 
//         exit(0); 
//     } 
//     else
//         printf("Socket successfully binded..\n"); 
  
//     // Now server is ready to listen and verification 
//     if ((listen(sockfd, 5)) != 0) { 
//         printf("Listen failed...\n"); 
//         exit(0); 
//     } 
//     else
//         printf("Server listening..\n"); 

//     len = sizeof(cli); 
  
//     // Accept the data packet from client and verification 
//     connfd = accept(sockfd, (SA*)&cli, &len); 
//     if (connfd < 0) { 
//         printf("server accept failed...\n"); 
//         exit(0); 
//     } 
//     else
//         printf("server accept the client...\n"); 
    
//     func(connfd); 


// }










// #include <unordered_map>
// #include <iostream>

// typedef struct Node* node;

// struct Node{
//     int data;
//     node next;
//     node prev;
// };

// struct Data{
//     int data;
//     node n;
// };

// class LinkedList{
//     public:
//         node start = NULL;
//         node last = NULL;
// };

// class LRU{
//     public:

//         int size,index = 0;
//         LinkedList list;
//         std::unordered_map<int,Data> cache;
        
//         LRU(int size_){
//             size = size_;
//         }

//         void swap(node &n1,node& n2){
//             int temp = n1->data;
//             n1->data = n2->data;
//             n2->data = temp;
//         }

//         int elementInCache(int key){
//             //element is in cache
//             auto it = cache.find(key);
//             return (it != cache.end());
//         }

//         int get(int key){
//             if (elementInCache(key)) return cache[key].data;
//             return -1;
//         }

//         void push(int key,int value){
//             if (!index){
//                 list.start = new Node{key,NULL,NULL};
//                 list.last = list.start;
//                 index++;
//                 cache.insert({key,{value,list.start}});
//                 return ;
//             }
//             else if (index < size){
//                 if (elementInCache(key)){
//                     node n = cache[key].n;
//                     node prev = n->prev,next = n->next;
//                     n->prev->next = next;
//                     next->prev = prev;
//                     list.last->next = n;
//                     n->prev = list.last;
//                     list.last = n;
//                     return;
//                 }
//                 list.last = new Node{key,NULL,list.last};
//                 list.last->prev->next = list.last;
//                 index++;
//                 cache.insert({key,{value,list.start}});
//                 return;
//             }
//             else{
//                 if (elementInCache(key)){
//                     node n = cache[key].n;
//                     node prev = n->prev,next = n->next;
//                     n->prev->next = next;
//                     next->prev = prev;
//                     list.last->next = n;
//                     n->prev = list.last;
//                     list.last = n;
//                     return;
//                 }
//                 node temp = list.start;
//                 if (!list.start->next){
//                     cache.erase(temp->data);
//                     delete temp;
//                     list.start = new Node{key,NULL,NULL};
//                     list.last = list.start;
//                     cache.insert({key,{value,list.start}});
//                     return ;
//                 }
//                 list.start = list.start->next;
//                 list.start->prev = NULL;
//                 cache.erase(temp->data);
//                 delete temp;
//                 list.last = new Node{key,nullptr,list.last};
//                 list.last->prev->next = list.last;
//                 cache.insert({key,{value,list.start}});
//             }
//         }
        

// };


// int main(){
//     LRU c(2);
//     c.push(1,3);
//     c.push(2,4);
//     c.push(3,5);
//     std::cout << c.get(1) << " ";
//     std::cout << c.get(2) << " ";
//     std::cout << c.get(3) << " ";
//     std::cout << c.list.start->data << " ";
//     // std::cout << list.start->next->data << " ";
//     // std::cout << list.start->next->next->data << " ";
//     std::cout << "hello world";
// }

