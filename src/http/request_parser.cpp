#include "request_parser.h"
#include "spdlog/spdlog.h"
#include "http/correlation_id_http_helper.h"
#include "httplib.h"
#include "messages/message_contract.h"
#include <unordered_set>
#include <ranges>

StatusRequest RequestParser::parseStatusRequest(const httplib::Request& request)
{
    const CorrelationId correlationId = CorrelationIdHttpHelper::extractCorrelationIdFromRequest(request);
    return StatusRequest{correlationId};
}

GetConfigRequest RequestParser::parseGetConfigRequest(const httplib::Request& request)
{
    const CorrelationId correlationId = CorrelationIdHttpHelper::extractCorrelationIdFromRequest(request);
    return GetConfigRequest{correlationId};
}

CalculateExchangeRequest RequestParser::parseCalculateExchangeRequest(const httplib::Request& request)
{
    const CorrelationId correlationId = CorrelationIdHttpHelper::extractCorrelationIdFromRequest(request);

    const auto sourceCurrencyCode = CurrencyCode(request.get_param_value(MessageContract::MessagePayload::CalculateExchangeRequestContract::SOURCE_CURRENCY));
    const auto targetCurrencyCode = CurrencyCode(request.get_param_value(MessageContract::MessagePayload::CalculateExchangeRequestContract::TARGET_CURRENCY));
    const auto sourceCurrencyAmount = CurrencyAmount(request.get_param_value(MessageContract::MessagePayload::CalculateExchangeRequestContract::SOURCE_CURRENCY_AMOUNT));

    return CalculateExchangeRequest{correlationId, sourceCurrencyCode, targetCurrencyCode, sourceCurrencyAmount};
}

UpdateCacheRequest RequestParser::parseUpdateCacheRequest(const httplib::Request& request)
{
    const CorrelationId correlationId = CorrelationIdHttpHelper::extractCorrelationIdFromRequest(request);
    return UpdateCacheRequest{correlationId};
}

bool RequestParser::hasRequiredCalculateExchangeRequestParameters(const httplib::Request& request)
{
    const std::unordered_set requiredParameters{
        MessageContract::MessagePayload::CalculateExchangeRequestContract::SOURCE_CURRENCY,
        MessageContract::MessagePayload::CalculateExchangeRequestContract::TARGET_CURRENCY,
        MessageContract::MessagePayload::CalculateExchangeRequestContract::SOURCE_CURRENCY_AMOUNT
    };

    if (request.params.size() != requiredParameters.size())
    {
        spdlog::warn("Bad CalculateExchange Request: Incorrect number of parameters (expected {}, got {})", requiredParameters.size(), request.params.size());
        return false;
    }

    for (const auto& parameter: requiredParameters)
    {
        if (!request.has_param(parameter))
        {
            spdlog::warn("Bad CalculateExchange Request: Missing required parameter '{}'", parameter);
            return false;
        }
    }

    for (const auto& key: request.params | std::views::keys)
    {
        if (!requiredParameters.contains(key))
        {
            spdlog::warn("Bad CalculateExchange Request: Unexpected parameter '{}'", key);
            return false;
        }
    }

    return true;
}
