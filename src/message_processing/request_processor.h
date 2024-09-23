#pragma once

#include <string>

class CurrenciesExchangeRateDatabank;
class DownloadManager;

class GetConfigRequest;
class CalculateExchangeRequest;
class UpdateCacheRequest;

class GetConfigResponse;
class CalculateExchangeResponse;
class UpdateCacheResponse;

class RequestProcessor
{
public:
    RequestProcessor() = delete;

    static GetConfigResponse processRequest(const GetConfigRequest& getConfigRequest, const CurrenciesExchangeRateDatabank& currenciesDatabank, const std::string& currenciesListFileContent);
    static CalculateExchangeResponse processRequest(const CalculateExchangeRequest& calculateExchangeRequest, const CurrenciesExchangeRateDatabank& currenciesDatabank);
    static UpdateCacheResponse processRequest(const UpdateCacheRequest& updateCacheRequest, CurrenciesExchangeRateDatabank& currenciesDatabank, DownloadManager& downloadManager);
};
