#include "currencies_exchange_rate_databank_modifier.h"
#include "currencies_exchange_rate_databank.h"
#include "types/currency_code.h"
#include "json_processing/json_parser.h"

void CurrenciesExchangeRateDatabankModifier::modifyCurrenciesExchangeRateDatabank(CurrenciesExchangeRateDatabank& currenciesExchangeRateDatabank, const std::map<CurrencyCode, ParseResult>& currencyCodeToParseResultMapping)
{
    for(const auto&[currencyCode, parseResult] : currencyCodeToParseResultMapping)
    {
        if(parseResult.isSuccess_)
        {
            currenciesExchangeRateDatabank.reassignAllExchangeRatesDataForCurrency(currencyCode, *parseResult.currencyCodeToCurrencyExchangeRateDataMapping_);
        }
    }
}
