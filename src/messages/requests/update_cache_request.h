#pragma once

#include "messages/abstract_request.h"

class UpdateCacheRequest : public AbstractRequest
{
public:
    explicit UpdateCacheRequest(const CorrelationId& correlationId) : AbstractRequest(correlationId)
    {}
};
