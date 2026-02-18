#pragma once

#include "types/correlation_id.h"
#include "messages/message_contract.h"
#include "httplib.h"
#include <string>

class CorrelationIdHttpHelper
{
public:
    static bool hasCorrelationId(const httplib::Request& request)
    {
        const auto it = request.headers.find(MessageContract::CORRELATION_ID_HEADER);
        return it != request.headers.end() && !it->second.empty();
    }

    static CorrelationId extractCorrelationIdFromRequest(const httplib::Request& request)
    {
        const auto it = request.headers.find(MessageContract::CORRELATION_ID_HEADER);

        if (it != request.headers.end() && !it->second.empty())
        {
            return CorrelationId(it->second);
        }

        throw std::runtime_error("Correlation ID header not found");
    }

    static void attachCorrelationIdToResponse(const CorrelationId& correlationId, httplib::Response& response)
    {
        response.set_header(MessageContract::CORRELATION_ID_HEADER, correlationId.toString());
    }
};
