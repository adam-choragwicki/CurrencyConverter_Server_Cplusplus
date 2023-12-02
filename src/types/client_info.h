#pragma once

#include <string>
#include "client_id.h"

class ClientInfo
{
public:
    ClientInfo(ClientId clientId, const std::string& address, unsigned int port, int socketFD);
    [[nodiscard]] const std::string& getAddress() const;
    [[nodiscard]] unsigned int getPort() const;
    [[nodiscard]] int getSocketFD() const;
    [[nodiscard]] ClientId getId() const;

private:
    ClientId id_;
    const std::string address_;
    const unsigned int port_;
    const int socketFD_;
};
