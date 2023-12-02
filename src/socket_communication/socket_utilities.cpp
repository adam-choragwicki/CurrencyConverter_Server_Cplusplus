#include "socket_utilities.h"
#include "exceptions.h"
#include <unistd.h>
#include <netinet/in.h>
#include "spdlog/spdlog.h"
#include "client_info_factory.h"

int SocketUtilities::createSocket()
{
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);

    if(socketFD != -1)
    {
        return socketFD;
    }
    else
    {
        logSocketError("Error occurred while creating socket");
        exit(EXIT_FAILURE);
    }
}

void SocketUtilities::setSocketOptions(int serverSocketFD)
{
    if(int option = 1; setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) != 0)
    {
        logSocketError("Error occurred while trying to set socket option (SO_REUSEADDR)");
        exit(EXIT_FAILURE);
    }
}

void SocketUtilities::bindSocketToAddress(int serverSocketFD, int port)
{
    struct sockaddr_in serverAddress_{};

    serverAddress_.sin_family = AF_INET;
    serverAddress_.sin_addr.s_addr = INADDR_ANY;
    serverAddress_.sin_port = htons(port);

    if(bind(serverSocketFD, (struct sockaddr*) &serverAddress_, sizeof(serverAddress_)) != 0)
    {
        logSocketError("Error occurred during socket bind call");
        exit(EXIT_FAILURE);
    }
}

void SocketUtilities::listenForConnection(int serverSocketFD)
{
    const int QUEUED_CONNECTIONS = 0;

    if(listen(serverSocketFD, QUEUED_CONNECTIONS) != 0)
    {
        logSocketError("Error occurred during socket listen call");
        exit(EXIT_FAILURE);
    }
}

ClientInfo SocketUtilities::acceptConnection(int serverSocketFD)
{
    struct sockaddr_in clientAddress{};
    int addressLength = sizeof(clientAddress);

    spdlog::info(" [*] Waiting for connection");

    int clientSocketFD = accept(serverSocketFD, (struct sockaddr*) &clientAddress, (socklen_t*) &addressLength);

    if(clientSocketFD >= 0)
    {
        spdlog::info(" [*] Connection accepted");
        ClientInfo clientInfo = ClientInfoFactory::makeClientInfo(clientAddress, clientSocketFD);
        spdlog::info("Client IP: {}, Port: {}, FD: {}", clientInfo.getAddress(), clientInfo.getPort(), clientInfo.getSocketFD());
        return clientInfo;
    }
    else
    {
        logSocketError("Error occurred during socket accept call");
        exit(EXIT_FAILURE);
    }
}

void SocketUtilities::sendMessage(int clientSocketFD, const std::string& message)
{
    /*Append new line character as message separator*/
    const std::string messageBody = message + "\n";

    const ssize_t bytesSent = send(clientSocketFD, messageBody.c_str(), messageBody.length(), 0);

    if(bytesSent == -1)
    {
        logSocketError("Error occurred while sending message");
        exit(EXIT_FAILURE);
    }
}

std::string SocketUtilities::receiveMessage(int clientSocketFD)
{
    char buffer[BUFFER_SIZE] = {0};

    long bytesRead = recv(clientSocketFD, buffer, BUFFER_SIZE, 0);

    if(bytesRead > 0)
    {
        return std::string({buffer, buffer + bytesRead});
    }
    else
    {
        throw ConnectionClosedByClient();
    }
}

void SocketUtilities::closeSocket(int socketFD)
{
    if(close(socketFD) == 0)
    {
        spdlog::info("Socket closed");
    }
    else
    {
        logSocketError("Error occurred during closing socket");
        exit(EXIT_FAILURE);
    }
}


void SocketUtilities::logSocketError(const std::string& message)
{
    spdlog::critical(message + ": " + strerror(errno));
}
