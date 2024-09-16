#include "currencies_exchange_rate_databank_updater.h"
#include "utilities/timer.h"
#include "downloader/download_manager.h"
#include "json_processing/json_parser.h"
#include "spdlog/spdlog.h"
#include "types/definitions.h"

bool CurrenciesExchangeRateDatabankUpdater::startCacheUpdate(CurrenciesExchangeRateDatabank& currenciesDatabank)
{
    spdlog::info("Starting currencies exchange rates update");

    Timer timer;

    CurrencyCodeToCurrencyExchangeRatesJsonMapping currenciesCodesToExchangeRatesJsonsMapping;

    try
    {
        currenciesCodesToExchangeRatesJsonsMapping = DownloadManager::getInstance().downloadCurrenciesExchangeRates(currenciesDatabank.getCurrenciesCodes());
    }
    catch(const DownloadError& exception)
    {
        spdlog::error(exception.what() + std::string(".\nCache update aborted"));
        return false;
    }

    spdlog::info("Downloaded all exchange rates");

    /*Get exchange rates timestamp from EUR-USD rate JSON because it is always present in the data*/
    Timestamp exchangeRatesTimestamp = JsonParser::parseTimestamp(CurrencyCode("eur"), currenciesCodesToExchangeRatesJsonsMapping.at(CurrencyCode("usd")));

    updateCache(currenciesCodesToExchangeRatesJsonsMapping, exchangeRatesTimestamp, currenciesDatabank);

    spdlog::info("Cache updated successfully in " + timer.getResult() + ". New exchange rates timestamp is " + exchangeRatesTimestamp.toString());

    return true;
}

void CurrenciesExchangeRateDatabankUpdater::updateCache(const CurrencyCodeToCurrencyExchangeRatesJsonMapping& currenciesCodesToExchangeRatesJsonsMapping, const Timestamp& exchangeRatesTimestamp, CurrenciesExchangeRateDatabank& currenciesDatabank)
{
    currenciesDatabank.setCache(currenciesCodesToExchangeRatesJsonsMapping);
    currenciesDatabank.setExchangeRatesTimestamp(exchangeRatesTimestamp);
}
