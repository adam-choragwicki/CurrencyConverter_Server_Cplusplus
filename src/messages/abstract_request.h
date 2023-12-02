#pragma once

#include "abstract_message.h"

class AbstractRequest : public AbstractMessage
{
public:
    AbstractRequest(const std::string& requestType, const MessageBody& messageBody, const CorrelationId& correlationId) : AbstractMessage(messageBody, correlationId)
    {}

    ~AbstractRequest() override = 0;
};
