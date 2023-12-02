#include "request_processor.h"
#include "calculate_exchange_request_validator.h"
#include "response_factory.h"
#include "converter/converter.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank_updater.h"
#include "spdlog/spdlog.h"

GetConfigResponse RequestProcessor::processRequest(const GetConfigRequest& getConfigRequest, const CurrenciesExchangeRateDatabank& currenciesDatabank)
{
    const Timestamp& currenciesExchangeRateTimestamp = currenciesDatabank.getCurrenciesExchangeRateTimestamp();
    const std::string& currenciesFileContent = currenciesDatabank.getCurrenciesFileContent();
    const CorrelationId& correlationId = getConfigRequest.getCorrelationId();

    auto getConfigResponse = ResponseFactory::makeGetConfigResponse(currenciesExchangeRateTimestamp, currenciesFileContent, correlationId);
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

    if(ValidationResult validationResult = CalculateExchangeRequestValidator::validateRequest(calculateExchangeRequest); validationResult.isValid())
    {
        if(currenciesDatabank.containsExchangeRate(sourceCurrencyCode, targetCurrencyCode))
        {
            status = MessageContract::MessageContent::CalculateExchangeResponseContract::OK_STATUS;
            calculationResult = Converter::convert(moneyAmount, currenciesDatabank.getExchangeRate(sourceCurrencyCode, targetCurrencyCode));
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

    auto calculateExchangeResponse = ResponseFactory::makeCalculateExchangeResponse(status, calculationResult, failureReason, correlationId);
    return calculateExchangeResponse;
}

UpdateCacheResponse RequestProcessor::processRequest(const UpdateCacheRequest& updateCacheRequest, CurrenciesExchangeRateDatabank& currenciesDatabank)
{
    bool updateSuccessful = CurrenciesExchangeRateDatabankUpdater::startCacheUpdate(currenciesDatabank);

    std::string status;
    Timestamp exchangeRatesTimestamp;

    const CorrelationId& correlationId = updateCacheRequest.getCorrelationId();

    if(updateSuccessful)
    {
        status = MessageContract::MessageContent::UpdateCacheResponseContract::OK_STATUS;
        exchangeRatesTimestamp = currenciesDatabank.getCurrenciesExchangeRateTimestamp();
    }
    else
    {
        status = MessageContract::MessageContent::UpdateCacheResponseContract::FAIL_STATUS;
        exchangeRatesTimestamp = Timestamp(MessageContract::MessageContent::NONE);
    }

    auto updateCacheResponse = ResponseFactory::makeUpdateCacheResponse(status, exchangeRatesTimestamp, correlationId);
    return updateCacheResponse;
}
