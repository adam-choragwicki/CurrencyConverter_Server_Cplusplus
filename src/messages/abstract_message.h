#pragma once

#include "types/correlation_id.h"

class AbstractMessage
{
public:
    explicit AbstractMessage(const CorrelationId& correlationId) : correlationId_(correlationId)
    {}

    virtual ~AbstractMessage() = 0;

    [[nodiscard]] const CorrelationId& getCorrelationId() const { return correlationId_; }

private:
    const CorrelationId correlationId_;
};
