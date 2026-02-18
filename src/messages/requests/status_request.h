#pragma once

#include "messages/abstract_request.h"

class StatusRequest : public AbstractRequest
{
public:
    explicit StatusRequest(const CorrelationId& correlationId) : AbstractRequest(correlationId)
    {}
};
