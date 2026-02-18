#pragma once

#include "types/exchange_rate.h"
#include "types/exchange_result.h"
#include "types/timestamp.h"
#include <optional>
#include <string>
#include <functional>
#include <utility>

class CurrenciesExchangeRatesDatabank;
class DownloadManager;
class CurrenciesNamesAndCodesFileContent;

class CalculateExchangeRequest;
class UpdateCacheRequest;

class ConfigData;

struct CalculateExchangeRequestProcessingResult
{
    static CalculateExchangeRequestProcessingResult success(const std::string& status,
                                                            const ExchangeRate& exchangeRate,
                                                            const ExchangeResult& exchangeResult,
                                                            const Timestamp& exchangeRateTimestamp)
    {
        return CalculateExchangeRequestProcessingResult(status,
                                                        true,
                                                        std::nullopt,
                                                        exchangeRate,
                                                        exchangeResult,
                                                        exchangeRateTimestamp);
    }

    static CalculateExchangeRequestProcessingResult failure(const std::string& status, std::string failureReason)
    {
        return CalculateExchangeRequestProcessingResult(status,
                                                        false,
                                                        std::move(failureReason),
                                                        std::nullopt,
                                                        std::nullopt,
                                                        std::nullopt);
    }

    std::string status;

    bool valid{};
    std::optional<std::string> failureReason{};

    std::optional<ExchangeRate> exchangeRate_;
    std::optional<ExchangeResult> exchangeResult_;
    std::optional<Timestamp> exchangeRateTimestamp_;

private:
    CalculateExchangeRequestProcessingResult(std::string status,
                                             bool valid,
                                             std::optional<std::string> failureReason,
                                             std::optional<ExchangeRate> exchangeRate,
                                             std::optional<ExchangeResult> exchangeResult,
                                             std::optional<Timestamp> exchangeRateTimestamp)
        : status(std::move(status)),
          valid(valid),
          failureReason(std::move(failureReason)),
          exchangeRate_(std::move(exchangeRate)),
          exchangeResult_(std::move(exchangeResult)),
          exchangeRateTimestamp_(std::move(exchangeRateTimestamp))
    {}
};

struct UpdateCacheRequestProcessingResult
{
    std::string status;
};

class RequestProcessor
{
public:
    RequestProcessor() = delete;

    static CalculateExchangeRequestProcessingResult processRequest(const CalculateExchangeRequest& calculateExchangeRequest, const CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank);
    static UpdateCacheRequestProcessingResult processRequest(const UpdateCacheRequest& updateCacheRequest,
                                                             CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank,
                                                             DownloadManager& downloadManager,
                                                             const std::function<void(size_t completed, size_t total)>& onProgress);
};
