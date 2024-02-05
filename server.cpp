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



class FileHandler {
public:
    static void handleGet(SOCKET clientSocket, const std::string& filename) {
        
    }

    static void handlePut(SOCKET clientSocket, const std::string& filename) {
        
    }

    static void handleList(SOCKET clientSocket) {
        std::string directory = "D:\\KSE\\paradigm\\Introduction to client-server programming using TCP\\Introduction-to-client-server-programming-using-TCP\\server\\Server\\serverfolder";
        std::ostringstream oss;

        if (!std::filesystem::exists(directory)) {
            oss << "Directory not found";
        }
        else {
            try {
                for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                    oss << entry.path().filename().string() << "\n";
                }
            }
            catch (const std::filesystem::filesystem_error& e) {
                oss << "Error accessing directory: " << e.what();
            }
        }

        std::string fileList = oss.str();
        send(clientSocket, fileList.c_str(), fileList.size(), 0);   
    }

    static void handleDelete(SOCKET clientSocket, const std::string& filename) {
        std::string fullPath = "D:\\KSE\\paradigm\\Introduction to client-server programming using TCP\\Introduction-to-client-server-programming-using-TCP\\server\\Server\\serverfolder\\" + filename;
        std::string response;
        if (std::filesystem::remove(fullPath)) {
            response = "File deleted successfully";
        }
        else {
            response = "Error deleting file";
        }
        send(clientSocket, response.c_str(), response.size(), 0);
    }

    static void handleInfo(SOCKET clientSocket, const std::string& filename) {
        std::string fullPath = "D:\\KSE\\paradigm\\Introduction to client-server programming using TCP\\Introduction-to-client-server-programming-using-TCP\\server\\Server\\serverfolder\\" + filename;
        std::ostringstream oss;
        if (std::filesystem::exists(fullPath)) {
            auto ftime = std::filesystem::last_write_time(fullPath);
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
            std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

            char timeString[26];
            std::tm tm;
            localtime_s(&tm, &cftime);
            asctime_s(timeString, sizeof(timeString), &tm);

            oss << "File: " << filename << "\nSize: " << std::filesystem::file_size(fullPath) << " bytes\n";
            oss << "Last modified: " << timeString;
        }
        else {
            oss << "File not found";
        }
        std::string fileInfo = oss.str();
        send(clientSocket, fileInfo.c_str(), fileInfo.size(), 0);
    }
};





class Server {
private:
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    std::vector<std::thread> clientThreads;

    void logConnection(const sockaddr_in& clientAddr) {
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
        std::ostringstream oss;
        oss << "Client connected: " << clientIP << ":" << ntohs(clientAddr.sin_port);
        std::cout << oss.str() << std::endl;
    }


    void handleClient(SOCKET clientSocket, sockaddr_in clientAddr) {
        logConnection(clientAddr);

        char buffer[4096];
        int bytesReceived;

        while (true) {
            ZeroMemory(buffer, 4096);
            bytesReceived = recv(clientSocket, buffer, 4096, 0);

            if (bytesReceived <= 0) break;

            std::string clientRequest(buffer, 0, bytesReceived);
            std::istringstream iss(clientRequest);
            std::string command, filename;
            iss >> command >> filename;

            if (command == "GET") {
                FileHandler::handleGet(clientSocket, filename);
            }
            else if (command == "PUT") {
                FileHandler::handlePut(clientSocket, filename);
            }
            else if (command == "LIST") {
                FileHandler::handleList(clientSocket);
            }
            else if (command == "DELETE") {
                FileHandler::handleDelete(clientSocket, filename);
            }
            else if (command == "INFO") {
                FileHandler::handleInfo(clientSocket, filename);
            }
            else {
                std::string errorMsg = "Invalid command";
                send(clientSocket, errorMsg.c_str(), errorMsg.size(), 0);
            }
        }

        closesocket(clientSocket);
    }


public:
    Server() : serverSocket(INVALID_SOCKET) {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            std::cerr << "WSAStartup failed: " << result << std::endl;
            exit(1);
        }
    }

    ~Server() {
        for (auto& thread : clientThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
        }
        WSACleanup();
    }

    bool startServer(int port) {
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
            return false;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            return false;
        }

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            return false;
        }

        std::cout << "Server is listening on port " << port << std::endl;
        return true;
    }

    void run() {
        while (true) {
            sockaddr_in clientAddr;
            int clientAddrSize = sizeof(clientAddr);

            SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
                continue;
            }
            clientThreads.push_back(std::thread(&Server::handleClient, this, clientSocket, clientAddr));
        }
    }
};

int main() {
    Server server;
    if (server.startServer(12345)) {
        server.run();
    }
    return 0;
}
