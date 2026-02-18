#pragma once

#include <string>

/*Contents of this file have to match the client counterpart*/

namespace MessageContract
{
    const std::string CORRELATION_ID_HEADER = "X-Correlation-ID";

    const std::string STATUS_ENDPOINT = "/status";
    const std::string GET_CONFIG_ENDPOINT = "/config";
    const std::string CALCULATE_EXCHANGE_ENDPOINT = "/calculate";
    const std::string UPDATE_CACHE_ENDPOINT = "/update";
    const std::string UPDATE_CACHE_PROGRESS_ENDPOINT = "/update/progress";

    const std::string OK_STATUS = "OK";

    namespace MessagePayload
    {
        namespace StatusRequestContract
        {}

        namespace StatusResponseContract
        {
            const std::string STATUS = "status";
        }

        namespace GetConfigRequestContract
        {}

        namespace GetConfigResponseContract
        {
            const std::string INITIAL_SOURCE_CURRENCY = "initial_source_currency";
            const std::string INITIAL_TARGET_CURRENCY = "initial_target_currency";
            const std::string CURRENCIES_NAMES_AND_CODES = "currencies_names_and_codes";
        }

        namespace CalculateExchangeRequestContract
        {
            const std::string SOURCE_CURRENCY = "source_currency";
            const std::string TARGET_CURRENCY = "target_currency";
            const std::string SOURCE_CURRENCY_AMOUNT = "source_currency_amount";
        }

        namespace CalculateExchangeResponseContract
        {
            const std::string SOURCE_CURRENCY = "source_currency";
            const std::string TARGET_CURRENCY = "target_currency";
            const std::string SOURCE_CURRENCY_AMOUNT = "source_currency_amount";
            const std::string EXCHANGE_RATE = "exchange_rate";
            const std::string EXCHANGE_RESULT = "exchange_result";
            const std::string EXCHANGE_RATE_TIMESTAMP = "exchange_rate_timestamp";

            const std::string FAIL_STATUS = "FAIL";
        }

        namespace UpdateCacheRequestContract
        {}

        namespace UpdateCacheResponseContract
        {
            const std::string STATUS = "status";
            const std::string FAIL_STATUS = "FAIL";
        }

        namespace UpdateCacheProgressResponseContract
        {
            const std::string STATUS = "status";
            const std::string COMPLETED = "completed";
            const std::string TOTAL = "total";

            const std::string IDLE_STATUS = "IDLE";
            const std::string IN_PROGRESS_STATUS = "IN_PROGRESS";
            const std::string OK_STATUS = "OK";
            const std::string FAIL_STATUS = "FAIL";
        }

        namespace ErrorResponseContract
        {
            const std::string ERROR_MESSAGE = "error";
            const std::string ERROR_CODE = "error_code";
        }

        namespace ErrorCodeContract
        {
            const std::string MISSING_CORRELATION_ID = "MISSING_CORRELATION_ID";
            const std::string UNEXPECTED_PARAMETERS = "UNEXPECTED_PARAMETERS";
            const std::string INVALID_PARAMETERS = "INVALID_PARAMETERS";
            const std::string INVALID_CALCULATION_REQUEST = "INVALID_CALCULATION_REQUEST";
            const std::string EXCHANGE_RATE_DATA_MISSING = "EXCHANGE_RATE_DATA_MISSING";
            const std::string UPDATE_CACHE_FAILED = "UPDATE_CACHE_FAILED";
            const std::string UPDATE_CACHE_ALREADY_IN_PROGRESS = "UPDATE_CACHE_ALREADY_IN_PROGRESS";
        }
    }
}
