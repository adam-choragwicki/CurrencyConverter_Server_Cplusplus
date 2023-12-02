#pragma once

#include "messages/abstract_response.h"
#include "types/client_info.h"

class ClientSocketHandler
{
public:
    explicit ClientSocketHandler(const ClientInfo& clientInfo);
    virtual ~ClientSocketHandler();
    void sendResponse(const AbstractResponse& response) const;
    [[nodiscard]] std::string receiveMessage() const;
    void closeSocket();
    [[nodiscard]] ClientId getId() const;
    [[nodiscard]] const ClientInfo& getClientInfo() const;

private:
    ClientInfo clientInfo_;
};
