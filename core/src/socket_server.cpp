#include "../include/socket_server.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>

bool SocketClient::connect_to_brain()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        std::cout << "Error creando socket\n";
        return false;
    }

    struct hostent* host = gethostbyname("localhost"); // ← CAMBIO AQUÍ

    if (host == nullptr)
    {
        std::cout << "Error resolviendo localhost\n";
        return false;
    }

    sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(65432);

    memcpy(&server.sin_addr, host->h_addr, host->h_length);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0)
    {
        std::cout << "No se pudo conectar al brain\n";
        return false;
    }

    std::cout << "Conectado al brain\n";

    return true;
}

std::string SocketClient::send_command(const std::string& command)
{
    send(sock, command.c_str(), command.size(), 0);

    char buffer[1024] = {0};

    recv(sock, buffer, 1024, 0);

    return std::string(buffer);
}