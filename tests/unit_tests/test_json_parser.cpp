#include <gtest/gtest.h>
#include "json_processing/json_parser.h"
#include "types/currencies_names_and_codes_file_content.h"
#include "types/currency_exchange_rates_json.h"
#include "types/currency_code.h"
#include "test_helpers.h"

TEST(JsonParserTest, ParsesConfigData)
{
    const std::string configJson = R"({"initial_source_currency":"eur","initial_target_currency":"usd"})";

    const ConfigData configData = JsonParser::parseConfigData(configJson);

    EXPECT_EQ(configData.initialSourceCurrency_, "eur");
    EXPECT_EQ(configData.initialTargetCurrency_, "usd");
}

TEST(JsonParserTest, ParsesCurrenciesNamesAndCodes)
{
    const std::string listJson = ReadFileToString(GetUnitTestDataDir() / "test_currencies_list.json");
    const CurrenciesNamesAndCodesFileContent fileContent(listJson);

    const std::set<CurrencyCode> currenciesCodes = JsonParser::parseCurrenciesNamesAndCodesFileToCurrenciesCodes(fileContent);

    EXPECT_EQ(currenciesCodes.size(), 4u);
    EXPECT_TRUE(currenciesCodes.contains(CurrencyCode("usd")));
    EXPECT_TRUE(currenciesCodes.contains(CurrencyCode("eur")));
    EXPECT_TRUE(currenciesCodes.contains(CurrencyCode("gbp")));
    EXPECT_TRUE(currenciesCodes.contains(CurrencyCode("pln")));
}

TEST(JsonParserTest, ParsesExchangeRatesJson)
{
    const std::string exchangeRatesJson = ReadFileToString(GetUnitTestDataDir() / "valid_usd.json");

    const std::set currenciesCodes{CurrencyCode("usd"), CurrencyCode("eur"), CurrencyCode("gbp"), CurrencyCode("pln")};
    const ParseResult parseResult = JsonParser::parseExchangeRatesJsonStringToCurrencyCodesToExchangeRateDataMapping(
        CurrencyCode("usd"),
        currenciesCodes,
        CurrencyExchangeRatesJson(exchangeRatesJson));

    ASSERT_TRUE(parseResult.isSuccess_);
    ASSERT_TRUE(parseResult.currencyCodeToCurrencyExchangeRateDataMapping_.has_value());

    const auto& mapping = *parseResult.currencyCodeToCurrencyExchangeRateDataMapping_;

    EXPECT_EQ(mapping.size(), 3u);
    EXPECT_FALSE(mapping.contains(CurrencyCode("usd")));
    EXPECT_TRUE(mapping.contains(CurrencyCode("eur")));
    EXPECT_TRUE(mapping.contains(CurrencyCode("gbp")));
    EXPECT_TRUE(mapping.contains(CurrencyCode("pln")));

    const double eurRate = std::stod(mapping.at(CurrencyCode("eur")).getExchangeRate().toString());
    EXPECT_NEAR(eurRate, 0.93140192399432, 1e-6);
}
