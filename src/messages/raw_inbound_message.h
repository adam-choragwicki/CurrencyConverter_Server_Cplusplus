#pragma once

#include "types/message_body.h"
#include "types/client_id.h"

class RawInboundMessage
{
public:
    explicit RawInboundMessage(const MessageBody& messageBody, const ClientId& senderId) : messageBody_(messageBody), senderId_(senderId)
    {}

    [[nodiscard]] const MessageBody& getMessageBody() const
    { return messageBody_; }

    [[nodiscard]] ClientId getSenderId() const
    { return senderId_; }

private:
    const MessageBody messageBody_;
    const ClientId senderId_;
};
