#include "inbound_message_queue.h"

void InboundMessageQueue::addMessage(const RawInboundMessage& rawInboundMessage)
{
    rawInboundMessages_.push(rawInboundMessage);
}

bool InboundMessageQueue::hasMessages() const
{
    return !rawInboundMessages_.empty();
}

RawInboundMessage InboundMessageQueue::popMessage()
{
    if(!rawInboundMessages_.empty())
    {
        RawInboundMessage rawInboundMessage = rawInboundMessages_.front();
        rawInboundMessages_.pop();
        return rawInboundMessage;
    }
    else
    {
        throw std::runtime_error("Error, trying to retrieve message from empty queue");
    }
}
