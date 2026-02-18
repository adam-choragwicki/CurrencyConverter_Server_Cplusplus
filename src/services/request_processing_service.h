#pragma once

#include "config_data.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "types/currencies_names_and_codes_file_content.h"
#include "downloader/download_manager.h"
#include "messages/requests/calculate_exchange_request.h"
#include "messages/requests/get_config_request.h"
#include "messages/requests/status_request.h"
#include "messages/requests/update_cache_request.h"
#include "messages/response_dto.h"
#include <memory>
#include <optional>
#include <string>
#include <utility>

class CacheUpdateState;

struct CalculateExchangeRequestResult
{
    static CalculateExchangeRequestResult setSuccess(CalculateExchangeResponseDto response)
    {
        return {true, false, std::nullopt, std::move(response)};
    }

    static CalculateExchangeRequestResult setFailure(const bool missingData, std::string failureReason)
    {
        return {false, missingData, std::move(failureReason), std::nullopt};
    }

    bool valid_{};
    bool missingData_{};
    std::optional<std::string> failureReason_;
    std::optional<CalculateExchangeResponseDto> response_;

private:
    CalculateExchangeRequestResult(const bool valid,
                                   const bool missingData,
                                   std::optional<std::string> failureReason,
                                   std::optional<CalculateExchangeResponseDto> response)
        : valid_(valid),
          missingData_(missingData),
          failureReason_(std::move(failureReason)),
          response_(std::move(response))
    {}
};

struct UpdateCacheRequestResult
{
    bool success{};
    bool alreadyInProgress{};
    UpdateCacheResponseDto response;
};

class IRequestProcessingService
{
public:
    virtual ~IRequestProcessingService() = default;
    [[nodiscard]] virtual StatusResponseDto handleStatus(const StatusRequest& request) const = 0;
    [[nodiscard]] virtual GetConfigResponseDto handleGetConfig(const GetConfigRequest& request) const = 0;
    [[nodiscard]] virtual CalculateExchangeRequestResult handleCalculate(const CalculateExchangeRequest& request) const = 0;
    [[nodiscard]] virtual UpdateCacheRequestResult handleUpdateCache(const UpdateCacheRequest& request) const = 0;
    [[nodiscard]] virtual UpdateCacheProgressResponseDto handleUpdateCacheProgress() const = 0;
};

class RequestProcessingService final : public IRequestProcessingService
{
public:
    RequestProcessingService(const ConfigData& configData,
                             const CurrenciesNamesAndCodesFileContent& currenciesNamesAndCodesFileContent,
                             CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank,
                             DownloadManager& downloadManager);

    [[nodiscard]] StatusResponseDto handleStatus(const StatusRequest& request) const override;
    [[nodiscard]] GetConfigResponseDto handleGetConfig(const GetConfigRequest& request) const override;
    [[nodiscard]] CalculateExchangeRequestResult handleCalculate(const CalculateExchangeRequest& request) const override;
    [[nodiscard]] UpdateCacheRequestResult handleUpdateCache(const UpdateCacheRequest& request) const override;
    [[nodiscard]] UpdateCacheProgressResponseDto handleUpdateCacheProgress() const override;

private:
    const ConfigData configData_;
    const CurrenciesNamesAndCodesFileContent currenciesNamesAndCodesFileContent_;
    CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank_;
    DownloadManager& downloadManager_;
    std::shared_ptr<CacheUpdateState> cacheUpdateState_;
};
