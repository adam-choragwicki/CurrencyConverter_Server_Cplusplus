#include "request_processing_service.h"
#include "json_processing/exceptions.h"
#include "json_processing/json_parser.h"
#include "message_processing/request_processor.h"
#include "messages/message_contract.h"
#include "services/cache_update_state.h"

RequestProcessingService::RequestProcessingService(const ConfigData& configData,
                                                   const CurrenciesNamesAndCodesFileContent& currenciesNamesAndCodesFileContent,
                                                   CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank,
                                                   DownloadManager& downloadManager)
    : configData_(configData),
      currenciesNamesAndCodesFileContent_(currenciesNamesAndCodesFileContent),
      currenciesExchangeRatesDatabank_(currenciesExchangeRatesDatabank),
      downloadManager_(downloadManager),
      cacheUpdateState_(std::make_shared<CacheUpdateState>())
{}

StatusResponseDto RequestProcessingService::handleStatus(const StatusRequest& request) const
{
    StatusResponseDto dto;

    dto.status = MessageContract::OK_STATUS;

    return dto;
}

GetConfigResponseDto RequestProcessingService::handleGetConfig(const GetConfigRequest& request) const
{
    GetConfigResponseDto dto;

    dto.initialSourceCurrencyCode = configData_.initialSourceCurrency_;
    dto.initialTargetCurrencyCode = configData_.initialTargetCurrency_;
    dto.currenciesNamesAndCodes = JsonParser::parseCurrenciesNamesAndCodesFileToMap(currenciesNamesAndCodesFileContent_);

    return dto;
}

CalculateExchangeRequestResult RequestProcessingService::handleCalculate(const CalculateExchangeRequest& request) const
{
    const CalculateExchangeRequestProcessingResult processingResult = RequestProcessor::processRequest(request, currenciesExchangeRatesDatabank_);

    if (!processingResult.valid)
    {
        const std::string& failureReason = processingResult.failureReason.value();
        const bool isMissingData = failureReason.starts_with("Exchange rate data missing");

        return CalculateExchangeRequestResult::setFailure(isMissingData, failureReason);
    }

    CalculateExchangeResponseDto dto;

    dto.status = processingResult.status;
    dto.sourceCurrencyCode = request.getSourceCurrencyCode().toString();
    dto.targetCurrencyCode = request.getTargetCurrencyCode().toString();
    dto.sourceCurrencyAmount = request.getSourceCurrencyAmount().toString();
    dto.exchangeRate = processingResult.exchangeRate_.value().toString();
    dto.exchangeResult = processingResult.exchangeResult_.value().toString();
    dto.exchangeRateTimestamp = processingResult.exchangeRateTimestamp_.value().toString();

    return CalculateExchangeRequestResult::setSuccess(dto);
}

UpdateCacheRequestResult RequestProcessingService::handleUpdateCache(const UpdateCacheRequest& request) const
{
    const size_t total = currenciesExchangeRatesDatabank_.getCurrenciesCodes().size();

    if (!cacheUpdateState_->tryBegin(total))
    {
        UpdateCacheResponseDto dto;
        dto.status = MessageContract::MessagePayload::UpdateCacheResponseContract::FAIL_STATUS;
        return UpdateCacheRequestResult{false, true, dto};
    }

    const auto onProgress = [this](const size_t completed, const size_t /*total*/)
    {
        cacheUpdateState_->reportCompleted(completed);
    };

    const UpdateCacheRequestProcessingResult processingResult = RequestProcessor::processRequest(request,
                                                                                                 currenciesExchangeRatesDatabank_,
                                                                                                 downloadManager_,
                                                                                                 onProgress);

    UpdateCacheResponseDto dto;
    dto.status = processingResult.status;

    const bool ok = processingResult.status == MessageContract::OK_STATUS;

    if (ok)
    {
        cacheUpdateState_->finishOk();
    }
    else
    {
        cacheUpdateState_->finishFail("Update cache failed");
    }

    return UpdateCacheRequestResult{ok, false, dto};
}

UpdateCacheProgressResponseDto RequestProcessingService::handleUpdateCacheProgress() const
{
    return cacheUpdateState_->snapshot();
}
