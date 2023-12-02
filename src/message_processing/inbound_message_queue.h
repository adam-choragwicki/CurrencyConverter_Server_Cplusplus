#pragma once

#include "messages/raw_inbound_message.h"
#include <queue>
#include <stdexcept>

class InboundMessageQueue
{
public:
    [[nodiscard]] bool hasMessages() const;
    void addMessage(const RawInboundMessage& rawInboundMessage);
    RawInboundMessage popMessage();

private:
    std::queue<RawInboundMessage> rawInboundMessages_;
};
