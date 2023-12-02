#include "client_info.h"

ClientInfo::ClientInfo(ClientId clientId, const std::string& address, unsigned int port, int socketFD) : id_(clientId), address_(address), port_(port), socketFD_(socketFD)
{}

const std::string& ClientInfo::getAddress() const
{
    return address_;
}

unsigned int ClientInfo::getPort() const
{
    return port_;
}

int ClientInfo::getSocketFD() const
{
    return socketFD_;
}

ClientId ClientInfo::getId() const
{
    return id_;
}
