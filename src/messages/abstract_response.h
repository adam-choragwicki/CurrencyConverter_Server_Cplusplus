#pragma once

#include "abstract_message.h"

class AbstractResponse : public AbstractMessage
{
public:
    AbstractResponse(const std::string& responseType, const MessageBody& messageBody, const CorrelationId& correlationId) : AbstractMessage(messageBody, correlationId), responseType(responseType)
    {}

    ~AbstractResponse() override = 0;

    [[nodiscard]] const std::string& getResponseType() const
    { return responseType; }

private:
    std::string responseType;
};
