#pragma once

#include "config_data.h"
#include "connection_data.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "types/currencies_names_and_codes_file_content.h"
#include <memory>

class DownloadManager;
class RequestProcessingService;
class StatusRequestHandler;
class GetConfigRequestHandler;
class CalculateExchangeRequestHandler;
class UpdateCacheRequestHandler;
class UpdateCacheProgressRequestHandler;

class Application
{
public:
    Application();
    ~Application();

private:
    ConfigData loadConfigData();
    ConnectionData loadConnectionData();
    CurrenciesNamesAndCodesFileContent loadCurrenciesNamesAndCodesFileContent();

    std::unique_ptr<CurrenciesExchangeRatesDatabank> currenciesExchangeRatesDatabank_;

    std::unique_ptr<DownloadManager> downloadManager_;
    std::unique_ptr<RequestProcessingService> requestProcessingService_;
    std::unique_ptr<StatusRequestHandler> statusRequestHandler_;
    std::unique_ptr<GetConfigRequestHandler> getConfigRequestHandler_;
    std::unique_ptr<CalculateExchangeRequestHandler> calculateExchangeRequestHandler_;
    std::unique_ptr<UpdateCacheRequestHandler> updateCacheRequestHandler_;
    std::unique_ptr<UpdateCacheProgressRequestHandler> updateCacheProgressRequestHandler_;
};
