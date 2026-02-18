#pragma once

#include "messages/requests/status_request.h"
#include "messages/requests/get_config_request.h"
#include "messages/requests/calculate_exchange_request.h"
#include "messages/requests/update_cache_request.h"
#include <stdexcept>

namespace httplib
{
    struct Request;
}

class CalculateExchangeRequestParseError : public std::runtime_error
{
public:
    explicit CalculateExchangeRequestParseError(const std::string& message) : std::runtime_error(message)
    {}
};

class RequestParser
{
public:
    static StatusRequest parseStatusRequest(const httplib::Request& request);
    static GetConfigRequest parseGetConfigRequest(const httplib::Request& request);
    static CalculateExchangeRequest parseCalculateExchangeRequest(const httplib::Request& request);
    static UpdateCacheRequest parseUpdateCacheRequest(const httplib::Request& request);

    static bool hasRequiredCalculateExchangeRequestParameters(const httplib::Request& request);
};
