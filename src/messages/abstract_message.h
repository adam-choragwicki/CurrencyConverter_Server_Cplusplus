#pragma once

#include "types/message_body.h"
#include "types/correlation_id.h"

class AbstractMessage
{
public:
    AbstractMessage(const MessageBody& messageBody, const CorrelationId& correlationId) : messageBody_(messageBody), correlationId_(correlationId)
    {}

    virtual ~AbstractMessage() = 0;

    [[nodiscard]] const MessageBody& getMessageBody() const
    { return messageBody_; }

    [[nodiscard]] const CorrelationId& getCorrelationId() const
    { return correlationId_; }

private:
    const MessageBody messageBody_;
    const CorrelationId correlationId_;
};
