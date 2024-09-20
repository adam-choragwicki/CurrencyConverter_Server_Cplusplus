#include "common_test_fixture.h"
#include "json_processing/json_parser.h"
#include "types/currency_code.h"
#include "types/currency_exchange_rates_json.h"
#include "types/containers/exchange_rate_data.h"
#include "utilities/file_loader.h"

class JsonParserTest : public CommonTestFixture
{
};

TEST_F(JsonParserTest, EmptyCurlyBraces)
{
    std::string emptyJsonString("{}");

    EXPECT_TRUE(JsonParser::isValidJsonString(emptyJsonString));
}

TEST_F(JsonParserTest, EmptyString)
{
    std::string emptyString;

    EXPECT_FALSE(JsonParser::isValidJsonString(emptyString));
}

TEST_F(JsonParserTest, MalformedString)
{
    std::string malformedString("{");

    EXPECT_FALSE(JsonParser::isValidJsonString(malformedString));
}

TEST_F(JsonParserTest, SimpleKeyValue)
{
    std::string jsonString = R"({"key" : "value"})";

    EXPECT_TRUE(JsonParser::isValidJsonString(jsonString));
}

TEST_F(JsonParserTest, UnquotedKey)
{
    std::string jsonString = R"({key : "value"})";

    EXPECT_FALSE(JsonParser::isValidJsonString(jsonString));
}

TEST_F(JsonParserTest, UnquotedNonStringValue)
{
    std::string jsonString = R"({"key" : value})";

    EXPECT_FALSE(JsonParser::isValidJsonString(jsonString));
}

TEST_F(JsonParserTest, NumericValue)
{
    std::string jsonString = R"({"key" : 5})";

    EXPECT_TRUE(JsonParser::isValidJsonString(jsonString));
}

TEST_F(JsonParserTest, BooleanValueLowerCase)
{
    std::string jsonString = R"({"key" : true})";

    EXPECT_TRUE(JsonParser::isValidJsonString(jsonString));
}

TEST_F(JsonParserTest, BooleanValueUpperCase)
{
    std::string jsonString = R"({"key" : True})";

    EXPECT_FALSE(JsonParser::isValidJsonString(jsonString));
}

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
    std::string currencyExchangeRatesJson = FileLoader::loadFileContent("test_data/jsons/valid_usd.json");

    std::set<CurrencyCode> currenciesCodes{CurrencyCode("usd"), CurrencyCode("eur"), CurrencyCode("gbp"), CurrencyCode("pln")};

    std::map<CurrencyCode, ExchangeRateData> currencyCodeToExchangeRateDataMapForUSD = JsonParser::parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(CurrencyCode("usd"),
                                                                                                                                                                        currenciesCodes,
                                                                                                                                                                        CurrencyExchangeRatesJson(currencyExchangeRatesJson));

    EXPECT_EQ(currencyCodeToExchangeRateDataMapForUSD.size(), 3);

    EXPECT_FALSE(currencyCodeToExchangeRateDataMapForUSD.contains(CurrencyCode("usd")));
    EXPECT_TRUE(currencyCodeToExchangeRateDataMapForUSD.contains(CurrencyCode("eur")));
    EXPECT_TRUE(currencyCodeToExchangeRateDataMapForUSD.contains(CurrencyCode("gbp")));
    EXPECT_TRUE(currencyCodeToExchangeRateDataMapForUSD.contains(CurrencyCode("pln")));
}
