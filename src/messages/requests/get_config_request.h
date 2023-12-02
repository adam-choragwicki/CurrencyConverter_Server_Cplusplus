#pragma once

#include "messages/abstract_request.h"
#include "messages/message_contract.h"

class GetConfigRequest : public AbstractRequest
{
public:
    GetConfigRequest(const MessageBody& messageBody, const CorrelationId& correlationId) : AbstractRequest(MessageContract::MessageType::RequestType::GET_CONFIG_REQUEST, messageBody, correlationId)
    {}
};
