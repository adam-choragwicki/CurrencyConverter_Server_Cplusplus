#include "request_processor.h"
#include "calculate_exchange_request_validator.h"
#include "exchange_calculator/exchange_calculator.h"
#include "types/exchange_result.h"

#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank_update_manager.h"
#include "messages/message_contract.h"

#include "messages/requests/calculate_exchange_request.h"
#include "messages/requests/update_cache_request.h"
#include "spdlog/spdlog.h"

CalculateExchangeRequestProcessingResult RequestProcessor::processRequest(const CalculateExchangeRequest& calculateExchangeRequest, const CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank)
{
    const CurrencyCode& sourceCurrencyCode = calculateExchangeRequest.getSourceCurrencyCode();
    const CurrencyCode& targetCurrencyCode = calculateExchangeRequest.getTargetCurrencyCode();
    const CurrencyAmount& currencyAmount = calculateExchangeRequest.getSourceCurrencyAmount();

    std::string status;
    if (const ValidationResult validationResult = CalculateExchangeRequestValidator::validateRequest(calculateExchangeRequest); !validationResult.isSuccess())
    {
        status = MessageContract::MessagePayload::CalculateExchangeResponseContract::FAIL_STATUS;
        const std::string& failureReason = validationResult.getFailureReason().value();

        return CalculateExchangeRequestProcessingResult::failure(status, failureReason);
    }

    // databank lookup
    const std::optional<ExchangeRateData> exchangeRateData = currenciesExchangeRatesDatabank.tryGetExchangeRateDataForCurrenciesPair(sourceCurrencyCode, targetCurrencyCode);

    if (!exchangeRateData)
    {
        const std::string failureReason = "Exchange rate data missing for pair: " + sourceCurrencyCode.toString() + " to " + targetCurrencyCode.toString();

        spdlog::warn(failureReason);

        return CalculateExchangeRequestProcessingResult::failure(status, failureReason);
    }

    status = MessageContract::OK_STATUS;

    const std::string rate = exchangeRateData->getExchangeRate().toString();

    // conversion
    try
    {
        const CurrencyAmount& sourceCurrencyAmount = calculateExchangeRequest.getSourceCurrencyAmount();
        const ExchangeResult exchangeResult = ExchangeCalculator::convert(sourceCurrencyAmount, exchangeRateData->getExchangeRate());

        if (status == MessageContract::OK_STATUS)
        {
            spdlog::info(currencyAmount.toString() + " " + sourceCurrencyCode.toUpperCase() + " = " + exchangeResult.toString() + " " + targetCurrencyCode.toUpperCase());
        }
        else
        {
            spdlog::error("Unexpected non-OK status during conversion: {}", status);
        }

        return CalculateExchangeRequestProcessingResult::success(status, exchangeRateData->getExchangeRate(), exchangeResult, exchangeRateData->getTimestamp());
    }
    catch (const ConversionError& e)
    {
        const std::string failureReason = std::format("Conversion error: {}", e.what());

        return CalculateExchangeRequestProcessingResult::failure(status, failureReason);
    }
}

UpdateCacheRequestProcessingResult RequestProcessor::processRequest(const UpdateCacheRequest& updateCacheRequest,
                                                                    CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank,
                                                                    DownloadManager& downloadManager,
                                                                    const std::function<void(size_t completed, size_t total)>& onProgress)
{
    const bool updateSuccessful = CurrenciesExchangeRatesDatabankUpdateManager::startCurrenciesExchangeRatesDatabankUpdate(currenciesExchangeRatesDatabank,
                                                                                                                           downloadManager,
                                                                                                                           onProgress);

    std::string status;

    if (updateSuccessful)
    {
        status = MessageContract::OK_STATUS;
    }
    else
    {
        status = MessageContract::MessagePayload::UpdateCacheResponseContract::FAIL_STATUS;
    }

    return UpdateCacheRequestProcessingResult(status);
}
