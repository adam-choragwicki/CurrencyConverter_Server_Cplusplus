#pragma once

#include "messages/abstract_response.h"

class CalculateExchangeResponse : public AbstractResponse
{
public:
    CalculateExchangeResponse(const std::string& responseType, const MessageBody& messageBody, const CorrelationId& correlationId) : AbstractResponse(responseType, messageBody, correlationId)
    {}
};
