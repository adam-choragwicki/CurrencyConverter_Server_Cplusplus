#pragma once

#include "messages/abstract_request.h"
#include "messages/message_contract.h"

class UpdateCacheRequest : public AbstractRequest
{
public:
    UpdateCacheRequest(const MessageBody& messageBody, const CorrelationId& correlationId) : AbstractRequest(MessageContract::MessageType::RequestType::UPDATE_CACHE_REQUEST, messageBody, correlationId)
    {}
};
