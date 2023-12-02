#include "connection_manager.h"
#include "spdlog/spdlog.h"

ConnectionManager::ConnectionManager(const Config& config)
{
    spdlog::debug("Initializing server socket");
    serverSocketHandler_ = std::make_unique<ServerSocketHandler>(config.getPort());
    spdlog::debug("Initializing server socket... DONE");
}

ClientSocketHandler& ConnectionManager::waitForConnectionAndAccept()
{
    ClientInfo newConnectedClientInfo = SocketUtilities::acceptConnection(serverSocketHandler_->getSocketFD());

    std::lock_guard<std::mutex> lock(mutex_);

    if(!clientSocketHandlers_.contains(newConnectedClientInfo.getId()))
    {
        auto[iter, _] = clientSocketHandlers_.try_emplace(newConnectedClientInfo.getId(), newConnectedClientInfo);
        return iter->second;
    }
    else
    {
        throw std::runtime_error("Duplicate client ID");
    }
}

void ConnectionManager::closeConnection(ClientSocketHandler& clientSocketHandler)
{
    std::lock_guard<std::mutex> lock(mutex_);
    clientSocketHandlers_.erase(clientSocketHandler.getId());
}

void ConnectionManager::sendResponse(const AbstractResponse& response, const ClientId& clientId) const
{
    if(clientSocketHandlers_.contains(clientId))
    {
        clientSocketHandlers_.at(clientId).sendResponse(response);
    }
    else
    {
        spdlog::warn("Client {} disconnected before receiving response", clientId.toString());
    }
}

std::vector<ClientInfo> ConnectionManager::getConnectedClients()
{
    std::vector<ClientInfo> connectedClients;

    for(const auto&[clientId, clientSocketHandler] : clientSocketHandlers_)
    {
        connectedClients.push_back(clientSocketHandler.getClientInfo());
    }

    return connectedClients;
}
