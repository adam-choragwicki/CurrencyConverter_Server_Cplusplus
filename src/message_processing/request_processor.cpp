#include "request_processor.h"
#include "calculate_exchange_request_validator.h"
#include "response_factory.h"
#include "converter/converter.h"
#include "spdlog/spdlog.h"

#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank_update_manager.h"

#include "messages/requests/get_config_request.h"
#include "messages/requests/calculate_exchange_request.h"
#include "messages/requests/update_cache_request.h"

#include "messages/responses/get_config_response.h"
#include "messages/responses/calculate_exchange_response.h"
#include "messages/responses/update_cache_response.h"

GetConfigResponse RequestProcessor::processRequest(const GetConfigRequest& getConfigRequest, const CurrenciesExchangeRateDatabank& currenciesDatabank, const std::string& currenciesListFileContent)
{
    const CorrelationId& correlationId = getConfigRequest.getCorrelationId();

    auto getConfigResponse = ResponseFactory::makeGetConfigResponse(currenciesListFileContent, correlationId);
    return getConfigResponse;
}

CalculateExchangeResponse RequestProcessor::processRequest(const CalculateExchangeRequest& calculateExchangeRequest, const CurrenciesExchangeRateDatabank& currenciesDatabank)
{
    const CurrencyCode& sourceCurrencyCode = calculateExchangeRequest.getSourceCurrencyCode();
    const CurrencyCode& targetCurrencyCode = calculateExchangeRequest.getTargetCurrencyCode();
    const MoneyAmount& moneyAmount = calculateExchangeRequest.getMoneyAmount();
    const CorrelationId& correlationId = calculateExchangeRequest.getCorrelationId();

    std::string status;
    std::string calculationResult;
    std::string failureReason;
    Timestamp currenciesExchangeRateTimestamp;

    if(ValidationResult validationResult = CalculateExchangeRequestValidator::validateRequest(calculateExchangeRequest); validationResult.isValid())
    {
        if(currenciesDatabank.containsExchangeRateData(sourceCurrencyCode, targetCurrencyCode))
        {
            status = MessageContract::MessageContent::CalculateExchangeResponseContract::OK_STATUS;

            const ExchangeRateData exchangeRateData = currenciesDatabank.getExchangeRateDataForCurrenciesPair(sourceCurrencyCode, targetCurrencyCode);

            calculationResult = Converter::convert(moneyAmount, exchangeRateData.getExchangeRate());
            currenciesExchangeRateTimestamp = exchangeRateData.getTimestamp();
            failureReason = MessageContract::MessageContent::NONE;
        }
        else
        {
            status = MessageContract::MessageContent::CalculateExchangeResponseContract::FAIL_STATUS;
            calculationResult = MessageContract::MessageContent::NONE;
            failureReason = "Source website does not provide exchange rate " + sourceCurrencyCode.toUpperCase() + "->" + targetCurrencyCode.toUpperCase();
        }
    }
    else
    {
        status = MessageContract::MessageContent::CalculateExchangeResponseContract::FAIL_STATUS;
        calculationResult = MessageContract::MessageContent::NONE;
        failureReason = validationResult.getFailureReason();
    }

    if(status == MessageContract::MessageContent::CalculateExchangeResponseContract::OK_STATUS)
    {
        spdlog::info(moneyAmount.toString() + " " + sourceCurrencyCode.toUpperCase() + " = " + calculationResult + " " + targetCurrencyCode.toUpperCase());
    }
    else
    {
        spdlog::error(failureReason);
    }

    auto calculateExchangeResponse = ResponseFactory::makeCalculateExchangeResponse(status, calculationResult, currenciesExchangeRateTimestamp, failureReason, correlationId);
    return calculateExchangeResponse;
}

UpdateCacheResponse RequestProcessor::processRequest(const UpdateCacheRequest& updateCacheRequest, CurrenciesExchangeRateDatabank& currenciesDatabank, DownloadManager& downloadManager)
{
    bool updateSuccessful = CurrenciesExchangeRateDatabankUpdateManager::startCurrenciesExchangeRateDatabankUpdate(currenciesDatabank, downloadManager);

    std::string status;

    if(updateSuccessful)
    {
        status = MessageContract::MessageContent::UpdateCacheResponseContract::OK_STATUS;
    }
    else
    {
        status = MessageContract::MessageContent::UpdateCacheResponseContract::FAIL_STATUS;
    }

    const CorrelationId& correlationId = updateCacheRequest.getCorrelationId();

    auto updateCacheResponse = ResponseFactory::makeUpdateCacheResponse(status, correlationId);
    return updateCacheResponse;
}
