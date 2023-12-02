#include "server_socket_handler.h"
#include "spdlog/spdlog.h"

ServerSocketHandler::ServerSocketHandler(int port) : serverSocketFD_(SocketUtilities::createSocket())
{
    SocketUtilities::setSocketOptions(serverSocketFD_);
    SocketUtilities::bindSocketToAddress(serverSocketFD_, port);
    SocketUtilities::listenForConnection(serverSocketFD_);
}

ServerSocketHandler::~ServerSocketHandler()
{
    spdlog::info("Closing server socket");
    SocketUtilities::closeSocket(serverSocketFD_);
}

int ServerSocketHandler::getSocketFD() const
{
    return serverSocketFD_;
}
