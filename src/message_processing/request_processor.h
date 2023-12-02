#pragma once

#include <string>

class CurrenciesExchangeRatesDatabank;
class DownloadManager;
class CurrenciesListFileContent;

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

    static GetConfigResponse processRequest(const GetConfigRequest& getConfigRequest, const CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank, const CurrenciesListFileContent& currenciesListFileContent);
    static CalculateExchangeResponse processRequest(const CalculateExchangeRequest& calculateExchangeRequest, const CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank);
    static UpdateCacheResponse processRequest(const UpdateCacheRequest& updateCacheRequest, CurrenciesExchangeRatesDatabank& currenciesExchangeRatesDatabank, DownloadManager& downloadManager);
};
