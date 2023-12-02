#pragma once

#include <netinet/in.h>
#include <arpa/inet.h>
#include "types/client_info.h"
#include "utilities/unique_id_generator.h"

class ClientInfoFactory
{
public:
    static ClientInfo makeClientInfo(const sockaddr_in& clientAddress, int clientSocketFD)
    {
        return {UniqueIdGenerator::generateID(), ClientInfoFactory::getIPAddress(clientAddress), ClientInfoFactory::getPort(clientAddress), clientSocketFD};
    }

private:
    static std::string getIPAddress(const sockaddr_in& clientAddress)
    {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddress.sin_addr), ip, INET_ADDRSTRLEN);
        return {ip};
    }

    static unsigned short getPort(const sockaddr_in& clientAddress)
    {
        return ntohs(clientAddress.sin_port);
    }
};
