#pragma once

#include <string>

/*Contents of this file have to match client counterpart*/

namespace MessageContract
{
    const std::string TYPE = "type";
    const std::string CORRELATION_ID = "correlation_id";

    namespace MessageType
    {
        namespace RequestType
        {
            const std::string GET_CONFIG_REQUEST = "get_config_request";
            const std::string CALCULATE_EXCHANGE_REQUEST = "calculate_exchange_request";
            const std::string UPDATE_CACHE_REQUEST = "update_cache_request";
        }

        namespace ResponseType
        {
            const std::string GET_CONFIG_RESPONSE = "get_config_response";
            const std::string CALCULATE_EXCHANGE_RESPONSE = "calculate_exchange_response";
            const std::string UPDATE_CACHE_RESPONSE = "update_cache_response";
        }
    }

    namespace MessageContent
    {
        const std::string NONE = "NULL";

        namespace GetConfigResponseContract
        {
            const std::string CURRENCIES_NAMES_AND_CODES = "currencies_names_and_codes";
        }

        namespace CalculateExchangeRequestContract
        {
            const std::string SOURCE_CURRENCY = "source_currency";
            const std::string TARGET_CURRENCY = "target_currency";
            const std::string MONEY_AMOUNT = "money_amount";
        }

        namespace CalculateExchangeResponseContract
        {
            const std::string STATUS = "status";
            const std::string CALCULATION_RESULT = "calculation_result";
            const std::string EXCHANGE_RATE_TIMESTAMP = "exchange_rate_timestamp";
            const std::string FAILURE_REASON = "failure_reason";

            const std::string OK_STATUS = "OK";
            const std::string FAIL_STATUS = "FAIL";
        }

        namespace UpdateCacheResponseContract
        {
            const std::string STATUS = "status";

            const std::string OK_STATUS = "OK";
            const std::string FAIL_STATUS = "FAIL";
        }
    }
}
