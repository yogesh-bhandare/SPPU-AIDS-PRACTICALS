#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>  

using namespace std; 

#pragma comment(lib, "ws2_32.lib")  

void start_tcp_server() {
    const char* host = "127.0.0.1";  
    const int port = 65432;          
    WSADATA wsaData;
    SOCKET serverSocket, connSocket;
    sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    char buffer[1024] = { 0 };

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed.\n";
        return;
    }

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed.\n";
        WSACleanup();
        return;
    }

    // Setup the server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Bind to any available address
    serverAddr.sin_port = htons(port);

    // Bind the socket
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Bind failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Listen failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
    cout << "TCP Server listening on " << host << ":" << port << "\n";

    // Accept a connection
    connSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
    if (connSocket == INVALID_SOCKET) {
        cerr << "Accept failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    cout << "Connected by client.\n";

    // Receive data from client
    while (true) {
        int bytesReceived = recv(connSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            break;  // Break on disconnect
        }
        cout << "Received from client: " << string(buffer, bytesReceived) << "\n";
        send(connSocket, buffer, bytesReceived, 0);  // Echo back the received data
    }

    // Close sockets
    closesocket(connSocket);
    closesocket(serverSocket);
    WSACleanup();
}

int main() {
    start_tcp_server();
    return 0;
}
