#include "currencies_exchange_rate_databank_updater.h"
#include "currencies_exchange_rate_databank.h"
#include "utilities/timer.h"
#include "downloader/download_manager.h"
#include "json_processing/json_parser.h"
#include "spdlog/spdlog.h"
#include "types/currency_code.h"
#include "types/definitions.h"
#include "types/currency_exchange_rates_json.h"

bool CurrenciesExchangeRateDatabankUpdater::startCacheUpdate(CurrenciesExchangeRateDatabank& currenciesDatabank, DownloadManager& downloadManager)
{
    spdlog::info("Starting currencies exchange rates update");

    //download
    //validate
    //parse

    Timer timer;

    CurrencyCodeToCurrencyExchangeRatesJsonMapping currenciesCodesToExchangeRatesJsonsMapping;


    //validate

    //    for(const auto&[currencyCode, currencyRateExchangeJson] : currenciesCodesToExchangeRatesJsonsMapping)
    //    {
    //        if(JsonReader::isValidJson(currencyRateExchangeJson.toString()))
    //        {
    //            currenciesDatabank.setExchangeRate(currencyCode, currencyRateExchangeJson);
    //        }
    //    }

    //    CurrencyCodeToCurrencyExchangeRatesJsonMappingValidator currenciesCodesToExchangeRatesJsonsMappingValidator;


    try
    {
        currenciesCodesToExchangeRatesJsonsMapping = downloadManager.downloadCurrenciesExchangeRates(currenciesDatabank.getCurrenciesCodes());
    }
    catch(const DownloadError& exception)
    {
        spdlog::error(exception.what() + std::string(".\nCache update aborted"));
        return false;
    }

    spdlog::info("Downloaded all exchange rates");

    //    /*Get exchange rates timestamp from EUR-USD rate JSON because it is always present in the data*/
    //    Timestamp exchangeRatesTimestamp = JsonParser::parseTimestamp(CurrencyCode("eur"), currenciesCodesToExchangeRatesJsonsMapping.at(CurrencyCode("usd")));

    //    updateCache(currenciesCodesToExchangeRatesJsonsMapping, exchangeRatesTimestamp, currenciesDatabank);

    currenciesDatabank.setCache(currenciesCodesToExchangeRatesJsonsMapping);

    //    spdlog::info("Cache updated successfully in " + timer.getResult() + ". New exchange rates timestamp is " + exchangeRatesTimestamp.toString());

    spdlog::info("Cache updated successfully in " + timer.getResult());

    return true;
}

void CurrenciesExchangeRateDatabankUpdater::updateCache(const CurrencyCodeToCurrencyExchangeRatesJsonMapping& currenciesCodesToExchangeRatesJsonsMapping, const Timestamp& exchangeRatesTimestamp, CurrenciesExchangeRateDatabank& currenciesDatabank)
{
}
