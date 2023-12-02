#pragma once

#include <arpa/inet.h>
#include "types/client_info.h"

class SocketUtilities
{
public:
    SocketUtilities() = delete;

    static int createSocket();
    static void setSocketOptions(int serverSocketFD);
    static void bindSocketToAddress(int serverSocketFD, int port);
    static void listenForConnection(int serverSocketFD);
    static ClientInfo acceptConnection(int serverSocketFD);
    static void sendMessage(int clientSocketFD, const std::string& message);
    static std::string receiveMessage(int clientSocketFD);
    static void closeSocket(int socketFD);

private:
    static const int BUFFER_SIZE = 1024;
    static void logSocketError(const std::string& message);
};
