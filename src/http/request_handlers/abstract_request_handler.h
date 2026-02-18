#pragma once

#include "httplib.h"
#include "types/correlation_id.h"
#include <optional>

class AbstractRequestHandler
{
protected:
    static std::optional<CorrelationId> ensureRequestHasCorrelationId(const httplib::Request& request, httplib::Response& response);

    template <typename Func>
    static void handleRequestIfItHasCorrelationId(const httplib::Request& request, httplib::Response& response, Func&& func)
    {
        const auto correlationId = ensureRequestHasCorrelationId(request, response);
        if (!correlationId)
        {
            return;
        }

        std::forward<Func>(func)(*correlationId);
    }

    static bool ensureRequestHasNoParameters(const httplib::Request& request,
                               httplib::Response& response,
                               const CorrelationId& correlationId,
                               const std::string& requestName);

    static void setJsonResponse(httplib::Response& response,
                                const CorrelationId& correlationId,
                                const std::string& responseContent);

    static void setErrorResponse(httplib::Response& response,
                                 int status,
                                 const std::string& errorCode,
                                 const std::string& message,
                                 const CorrelationId* correlationId = nullptr);
};
