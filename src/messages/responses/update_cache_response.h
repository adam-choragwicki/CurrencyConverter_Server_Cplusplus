#pragma once

#include "messages/abstract_response.h"

class UpdateCacheResponse : public AbstractResponse
{
public:
    UpdateCacheResponse(const std::string& responseType, const MessageBody& messageBody, const CorrelationId& correlationId) : AbstractResponse(responseType, messageBody, correlationId)
    {}
};
