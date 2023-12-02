#pragma once

#include "types/message_body.h"
#include "correlation_id_stub.h"
#include "messages/responses/get_config_response.h"

class ResponseStub
{
public:
    static GetConfigResponse getValue(const std::string& message)
    {
        return {"response type", MessageBody(message), CorrelationIdStub::getValue()};
    }
};
