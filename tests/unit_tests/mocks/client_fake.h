#pragma once

#include <arpa/inet.h>
#include "socket_communication/socket_utilities.h"
#include "spdlog/spdlog.h"

class ClientFake
{
public:
    explicit ClientFake(int port)
    {
        clientSocketFD_ = socket(AF_INET, SOCK_STREAM, 0);

        if(clientSocketFD_ == -1)
        {
            std::cerr << "Error occurred while creating socket: " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }

        serverAddress_.sin_family = AF_INET;
        serverAddress_.sin_port = htons(port);
    }

    void connectToServer()
    {
        if(connect(clientSocketFD_, (struct sockaddr*) &serverAddress_, sizeof(serverAddress_)) == 0)
        {
            spdlog::info("Client connected to server");
        }
        else
        {
            std::cerr << "Error occurred while trying to connect to server: " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void disconnectFromServer()
    {
        if(close(clientSocketFD_) == 0)
        {
            clientSocketFD_ = -1;
            spdlog::info("Client socket closed from client's side");
        }
        else
        {
            std::cerr << "Error occurred during closing client socket: " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void sendMessage(const std::string& message) const
    {
        const std::string messageBody = message + "\n";
        send(clientSocketFD_, messageBody.c_str(), messageBody.length(), 0);
    }

    [[nodiscard]] std::string receiveMessage() const
    {
        return SocketUtilities::receiveMessage(clientSocketFD_);
    }

private:
    int clientSocketFD_{-1};
    struct sockaddr_in serverAddress_{};
};
