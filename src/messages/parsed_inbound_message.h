#pragma once

#include "types/message_body.h"
#include "types/message_type.h"
#include "types/client_id.h"

class ParsedInboundMessage
{
public:
    explicit ParsedInboundMessage(const MessageType& messageType, const MessageBody& messageBody, const ClientId& senderId, const CorrelationId& correlationId) :
            messageType_(messageType), messageBody_(messageBody), senderId_(senderId), correlationId_(correlationId)
    {}

    [[nodiscard]] const MessageType& getMessageType() const
    { return messageType_; }

    [[nodiscard]] const MessageBody& getMessageBody() const
    { return messageBody_; }

    [[nodiscard]] ClientId getSenderId() const
    { return senderId_; }

    [[nodiscard]] const CorrelationId& getCorrelationId() const
    { return correlationId_; }

private:
    const MessageType messageType_;
    const MessageBody messageBody_;
    const ClientId senderId_;
    const CorrelationId correlationId_;
};
