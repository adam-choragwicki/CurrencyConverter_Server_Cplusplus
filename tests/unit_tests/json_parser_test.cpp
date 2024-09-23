#include "common_test_fixture.h"
#include "types/currency_code.h"
#include "types/currency_exchange_rates_json.h"
#include "utilities/files_helper.h"
#include "json_processing/json_parser.h"
#include "types/currencies_list_file_content.h"
#include "stubs/currencies_list_file_content_stub.h"

class JsonParserTest : public CommonTestFixture
{
};

TEST_F(JsonParserTest, ParseCurrenciesListFileContentToCurrenciesCodes)
{
    CurrenciesListFileContent currenciesListFileContent = CurrenciesListFileContentStub::getValue();

    std::set<CurrencyCode> currenciesCodes = JsonParser::parseCurrenciesListFileContentToCurrenciesCodes(currenciesListFileContent);

    EXPECT_EQ(currenciesCodes.size(), 3);

    EXPECT_TRUE(currenciesCodes.contains(CurrencyCode("usd")));
    EXPECT_TRUE(currenciesCodes.contains(CurrencyCode("eur")));
    EXPECT_TRUE(currenciesCodes.contains(CurrencyCode("gbp")));
}

TEST_F(JsonParserTest, ParseExchangeRatesJsonStringToCurrencyCodesToExchangeRatesDataMapping)
{
    std::string currencyExchangeRatesJson = FilesHelper::loadFileContent("data/jsons/valid_usd.json");

    std::set<CurrencyCode> currenciesCodes{CurrencyCode("usd"), CurrencyCode("eur"), CurrencyCode("gbp"), CurrencyCode("pln")};

    ParseResult parseResult = JsonParser::parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(CurrencyCode("usd"), currenciesCodes, CurrencyExchangeRatesJson(currencyExchangeRatesJson));

    CurrencyCodeToCurrencyExchangeRateDataMapping currencyCodeToExchangeRateDataMapForUSD = *parseResult.currencyCodeToCurrencyExchangeRateDataMapping_;

    EXPECT_EQ(currencyCodeToExchangeRateDataMapForUSD.size(), 3);

    EXPECT_FALSE(currencyCodeToExchangeRateDataMapForUSD.contains(CurrencyCode("usd")));
    EXPECT_TRUE(currencyCodeToExchangeRateDataMapForUSD.contains(CurrencyCode("eur")));
    EXPECT_TRUE(currencyCodeToExchangeRateDataMapForUSD.contains(CurrencyCode("gbp")));
    EXPECT_TRUE(currencyCodeToExchangeRateDataMapForUSD.contains(CurrencyCode("pln")));
}
