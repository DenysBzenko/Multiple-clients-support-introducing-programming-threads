#include <iostream>
#include <WinSock2.h>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <thread>
#include <ws2tcpip.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")


class Client {
private:
    SOCKET clientSocket;
    sockaddr_in serverAddr;

public:
    Client() : clientSocket(INVALID_SOCKET) {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            std::cerr << "WSAStartup failed: " << result << std::endl;
            exit(1);
        }
    }

    ~Client() {
        if (clientSocket != INVALID_SOCKET) {
            closesocket(clientSocket);
        }
        WSACleanup();
    }
    bool connectServer(const std::string& ip, int port) {
        clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
            return false;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);

       
        if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
            std::cerr << "Invalid address/ Address not supported" << std::endl;
            closesocket(clientSocket);
            return false;
        }

        if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Failed to connect to server: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            return false;
        }

        std::cout << "Connected to server." << std::endl;
        return true;
    }


    void sendCommand(const std::string& command) {
        send(clientSocket, command.c_str(), command.size(), 0);


        char buffer[4096];
        int bytesReceived = recv(clientSocket, buffer, 4096, 0);
        if (bytesReceived > 0) {
            std::cout << "SERVER> " << std::string(buffer, 0, bytesReceived) << std::endl;
        }
    }
};

int main() {
    Client client;
    if (client.connectServer("127.0.0.1", 12345)) {
        std::string command;
        while (true) {
            std::cout << "Enter command: ";
            std::getline(std::cin, command);
            if (command == "exit") {
                break;
            }
            client.sendCommand(command);
        }
    }
    return 0;
}
