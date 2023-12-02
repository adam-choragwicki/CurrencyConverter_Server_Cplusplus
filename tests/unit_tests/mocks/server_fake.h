#pragma once

#include <arpa/inet.h>
#include "socket_communication/socket_utilities.h"

class ServerFake
{
public:
    explicit ServerFake(int port) : serverSocketFD_(SocketUtilities::createSocket())
    {
        SocketUtilities::setSocketOptions(serverSocketFD_);
        SocketUtilities::bindSocketToAddress(serverSocketFD_, port);
        SocketUtilities::listenForConnection(serverSocketFD_);
    }

    virtual ~ServerFake()
    {
        SocketUtilities::closeSocket(serverSocketFD_);
    }

    [[nodiscard]] ClientInfo acceptConnection() const
    {
        ClientInfo newConnectedClientInfo = SocketUtilities::acceptConnection(serverSocketFD_);
        return newConnectedClientInfo;
    }

private:
    const int serverSocketFD_{-1};
};
