#include "common_test_fixture.h"
#include "types/currency_code.h"
#include "types/currency_exchange_rates_json.h"
#include "utilities/files_helper.h"
#include "json_processing/json_parser.h"

class JsonParserTest : public CommonTestFixture
{
};

TEST_F(JsonParserTest, ParseCurrenciesListFileContentToCurrenciesCodes)
{
    std::string currenciesListFileContent = R"({
        "U.S. Dollar": "USD",
        "Euro": "EUR",
        "U.K. Pound Sterling": "GBP",
        "Polish Zloty": "PLN"
    })";

    std::set<CurrencyCode> currenciesCodes = JsonParser::parseCurrenciesListFileContentToCurrenciesCodes(currenciesListFileContent);

    EXPECT_EQ(currenciesCodes.size(), 4);

    EXPECT_TRUE(currenciesCodes.contains(CurrencyCode("usd")));
    EXPECT_TRUE(currenciesCodes.contains(CurrencyCode("eur")));
    EXPECT_TRUE(currenciesCodes.contains(CurrencyCode("gbp")));
    EXPECT_TRUE(currenciesCodes.contains(CurrencyCode("pln")));
}

TEST_F(JsonParserTest, ParseExchangeRatesJsonStringToCurrencyCodesToExchangeRatesDataMapping)
{
    std::string currencyExchangeRatesJson = FilesHelper::loadFileContent("data/valid_usd.json");

    std::set<CurrencyCode> currenciesCodes{CurrencyCode("usd"), CurrencyCode("eur"), CurrencyCode("gbp"), CurrencyCode("pln")};

    ParseResult parseResult = JsonParser::parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(CurrencyCode("usd"), currenciesCodes, CurrencyExchangeRatesJson(currencyExchangeRatesJson));

    CurrencyCodeToCurrencyExchangeRateDataMapping currencyCodeToExchangeRateDataMapForUSD = *parseResult.currencyCodeToCurrencyExchangeRateDataMapping_;

    EXPECT_EQ(currencyCodeToExchangeRateDataMapForUSD.size(), 3);

    EXPECT_FALSE(currencyCodeToExchangeRateDataMapForUSD.contains(CurrencyCode("usd")));
    EXPECT_TRUE(currencyCodeToExchangeRateDataMapForUSD.contains(CurrencyCode("eur")));
    EXPECT_TRUE(currencyCodeToExchangeRateDataMapForUSD.contains(CurrencyCode("gbp")));
    EXPECT_TRUE(currencyCodeToExchangeRateDataMapForUSD.contains(CurrencyCode("pln")));
}
