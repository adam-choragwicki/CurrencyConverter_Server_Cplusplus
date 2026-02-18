#pragma once

#include "abstract_message.h"

class AbstractRequest : public AbstractMessage
{
public:
    explicit AbstractRequest(const CorrelationId& correlationId) : AbstractMessage(correlationId)
    {}

    ~AbstractRequest() override = 0;
};
