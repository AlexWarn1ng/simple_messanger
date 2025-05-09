#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

std::vector<SOCKET> clients;
std::mutex clients_mutex;

void handle_client(SOCKET client_socket, sockaddr_in client_addr) {
    char buffer[1024];
    char client_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    std::cout << "Client connected: " << client_ip
        << ":" << ntohs(client_addr.sin_port) << std::endl;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {
            std::cout << "Client disconnected: " << client_ip
                << ":" << ntohs(client_addr.sin_port) << std::endl;
            break;
        }

        std::string message(buffer);
        std::cout << "Message from client " << client_ip << ": " << message << std::endl;

        // Broadcast message to all clients
        clients_mutex.lock();
        for (SOCKET other_client : clients) {
            if (other_client != client_socket) {
                send(other_client, buffer, bytes_received, 0);
            }
        }
        clients_mutex.unlock();
    }

    // Remove client from list
    clients_mutex.lock();
    clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
    clients_mutex.unlock();

    closesocket(client_socket);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5555);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // requesting info
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        std::cout << "Hostname: " << hostname << std::endl;

        ADDRINFOA hints = { 0 };
        hints.ai_family = AF_INET;
        ADDRINFOA* result = nullptr;

        if (getaddrinfo(hostname, nullptr, &hints, &result) == 0) {
            std::cout << "Available IP addresses:" << std::endl;
            for (ADDRINFOA* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
                char ip_str[INET_ADDRSTRLEN];
                sockaddr_in* sockaddr_ipv4 = (sockaddr_in*)ptr->ai_addr;
                inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), ip_str, INET_ADDRSTRLEN);
                std::cout << ip_str << std::endl;
            }
            freeaddrinfo(result);
        }
    }

    // requesting port
    sockaddr_in sin;
    int addrlen = sizeof(sin);
    if (getsockname(server_socket, (sockaddr*)&sin, &addrlen) == 0) {
        std::cout << "Server port: " << ntohs(sin.sin_port) << std::endl;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server started. Waiting for connections..." << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    while (true) {
        sockaddr_in client_addr;
        int client_size = sizeof(client_addr);
        SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_size);

        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        clients_mutex.lock();
        clients.push_back(client_socket);
        clients_mutex.unlock();

        std::thread(handle_client, client_socket, client_addr).detach();
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}