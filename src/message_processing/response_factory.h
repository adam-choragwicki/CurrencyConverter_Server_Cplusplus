#pragma once

#include "messages/responses/get_config_response.h"
#include "messages/responses/calculate_exchange_response.h"
#include "messages/responses/update_cache_response.h"
#include "json_processing/json_writer.h"
#include "types/timestamp.h"

class ResponseFactory
{
public:
    static GetConfigResponse makeGetConfigResponse(const std::string& currenciesNamesAndCodes, const CorrelationId& correlationId);
    static CalculateExchangeResponse makeCalculateExchangeResponse(const std::string& status, const std::string& calculationResult, const Timestamp& exchangeRateTimestamp, const std::string& failureReason, const CorrelationId& correlationId);
    static UpdateCacheResponse makeUpdateCacheResponse(const std::string& status, const CorrelationId& correlationId);

private:
    static void assignResponseType(JsonWriter& jsonWriter, const std::string& type);
    static void assignCorrelationId(JsonWriter& jsonWriter, const CorrelationId& correlationId);
};
