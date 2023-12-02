#pragma once

#include "messages/abstract_response.h"

class GetConfigResponse : public AbstractResponse
{
public:
    GetConfigResponse(const std::string& responseType, const MessageBody& messageBody, const CorrelationId& correlationId) : AbstractResponse(responseType, messageBody, correlationId)
    {}
};
