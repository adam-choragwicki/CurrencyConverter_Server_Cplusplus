#include "client_socket_handler.h"
#include "socket_utilities.h"
#include "spdlog/spdlog.h"

ClientSocketHandler::ClientSocketHandler(const ClientInfo& clientInfo) : clientInfo_(clientInfo)
{}

ClientSocketHandler::~ClientSocketHandler()
{
    closeSocket();
}

void ClientSocketHandler::sendResponse(const AbstractResponse& response) const
{
    SocketUtilities::sendMessage(clientInfo_.getSocketFD(), response.getMessageBody().toString());
    spdlog::info(" [->] Sending " + response.getResponseType() + " [" + response.getCorrelationId().toString() + "]");
    spdlog::debug(std::to_string(response.getMessageBody().toString().size()) + " bytes: " + response.getMessageBody().toString());
}

std::string ClientSocketHandler::receiveMessage() const
{
    return SocketUtilities::receiveMessage(clientInfo_.getSocketFD());
}

void ClientSocketHandler::closeSocket()
{
    spdlog::info("Closing client socket, IP: {}, Port: {}, FD: {}", clientInfo_.getAddress(), clientInfo_.getPort(), clientInfo_.getSocketFD());

    SocketUtilities::closeSocket(clientInfo_.getSocketFD());
}

ClientId ClientSocketHandler::getId() const
{
    return clientInfo_.getId();
}

const ClientInfo& ClientSocketHandler::getClientInfo() const
{
    return clientInfo_;
}
