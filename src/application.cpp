#include "application.h"
#include "http/server_manager.h"
#include "utilities/file_helper.h"
#include "paths.h"
#include "json_processing/json_parser.h"
#include "log_manager.h"
#include "spdlog/spdlog.h"
#include "downloader/download_manager.h"
#include "http/request_handlers/status_request_handler.h"
#include "http/request_handlers/get_config_request_handler.h"
#include "http/request_handlers/calculate_exchange_request_handler.h"
#include "http/request_handlers/update_cache_request_handler.h"
#include "http/request_handlers/update_cache_progress_request_handler.h"
#include "services/request_processing_service.h"

Application::Application()
{
    LogManager::setupLogging();

    spdlog::info("Starting currency converter server...");

    const ConnectionData connectionData = loadConnectionData();
    const ConfigData configData = loadConfigData();

    const CurrenciesNamesAndCodesFileContent currenciesNamesAndCodesFileContent = loadCurrenciesNamesAndCodesFileContent();
    // spdlog::debug("{} file content: {}", Paths::CURRENCIES_NAMES_AND_CODES_FILE_PATH, currenciesNamesAndCodesFileContent_.toString());

    currenciesExchangeRatesDatabank_ = std::make_unique<CurrenciesExchangeRatesDatabank>(currenciesNamesAndCodesFileContent);

    spdlog::debug("Currencies exchange rate databank initialized");

    downloadManager_ = std::make_unique<DownloadManager>();

    requestProcessingService_ = std::make_unique<RequestProcessingService>(configData,
                                                                           currenciesNamesAndCodesFileContent,
                                                                           *currenciesExchangeRatesDatabank_,
                                                                           *downloadManager_);

    statusRequestHandler_ = std::make_unique<StatusRequestHandler>(*requestProcessingService_);
    getConfigRequestHandler_ = std::make_unique<GetConfigRequestHandler>(*requestProcessingService_);
    calculateExchangeRequestHandler_ = std::make_unique<CalculateExchangeRequestHandler>(*requestProcessingService_);
    updateCacheRequestHandler_ = std::make_unique<UpdateCacheRequestHandler>(*requestProcessingService_);
    updateCacheProgressRequestHandler_ = std::make_unique<UpdateCacheProgressRequestHandler>(*requestProcessingService_);

    ServerManager serverManager(connectionData.host,
                                connectionData.port,
                                *statusRequestHandler_,
                                *getConfigRequestHandler_,
                                *calculateExchangeRequestHandler_,
                                *updateCacheRequestHandler_,
                                *updateCacheProgressRequestHandler_);

    serverManager.listen(); // blocking call
}

Application::~Application()
{
    spdlog::info("Server stopped cleanly");
}

ConfigData Application::loadConfigData()
{
    const std::string configFileContent = FileHelper::loadFileContent(Paths::CONFIG_FILE_PATH);
    return JsonParser::parseConfigData(configFileContent);
}

ConnectionData Application::loadConnectionData()
{
    const std::string connectionFileContent = FileHelper::loadFileContent(Paths::CONNECTION_FILE_PATH);
    return JsonParser::parseConnectionData(connectionFileContent);
}

CurrenciesNamesAndCodesFileContent Application::loadCurrenciesNamesAndCodesFileContent()
{
    return CurrenciesNamesAndCodesFileContent(FileHelper::loadFileContent(Paths::CURRENCIES_NAMES_AND_CODES_FILE_PATH));
}
