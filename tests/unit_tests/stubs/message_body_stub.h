#pragma once

#include "types/message_body.h"

class MessageBodyStub
{
public:
    static MessageBody getValue()
    {
        return MessageBody{R"({"type":"get_config_request", "correlation_id":"1234"})"};
    }
};
