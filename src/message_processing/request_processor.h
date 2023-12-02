#pragma once

#include "messages/requests/get_config_request.h"
#include "messages/requests/calculate_exchange_request.h"
#include "messages/requests/update_cache_request.h"
#include "messages/responses/get_config_response.h"
#include "messages/responses/calculate_exchange_response.h"
#include "messages/responses/update_cache_response.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"

class RequestProcessor
{
public:
    RequestProcessor() = delete;

    static GetConfigResponse processRequest(const GetConfigRequest& getConfigRequest, const CurrenciesExchangeRateDatabank& currenciesDatabank);
    static CalculateExchangeResponse processRequest(const CalculateExchangeRequest& calculateExchangeRequest, const CurrenciesExchangeRateDatabank& currenciesDatabank);
    static UpdateCacheResponse processRequest(const UpdateCacheRequest& updateCacheRequest, CurrenciesExchangeRateDatabank& currenciesDatabank);
};
