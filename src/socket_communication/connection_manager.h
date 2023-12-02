#pragma once

#include "server_socket_handler.h"
#include "client_socket_handler.h"
#include "config/config.h"
#include <map>
#include <memory>
#include <vector>

class ConnectionManager
{
public:
    explicit ConnectionManager(const Config& config);
    ClientSocketHandler& waitForConnectionAndAccept();
    void closeConnection(ClientSocketHandler& clientSocketHandler);
    void sendResponse(const AbstractResponse& response, const ClientId& clientId) const;
    std::vector<ClientInfo> getConnectedClients();

private:
    std::unique_ptr<ServerSocketHandler> serverSocketHandler_;
    std::map<ClientId, ClientSocketHandler> clientSocketHandlers_;
    std::mutex mutex_;
};
