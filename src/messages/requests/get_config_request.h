#pragma once

#include "messages/abstract_request.h"

class GetConfigRequest : public AbstractRequest
{
public:
    explicit GetConfigRequest(const CorrelationId& correlationId) : AbstractRequest(correlationId)
    {}
};
