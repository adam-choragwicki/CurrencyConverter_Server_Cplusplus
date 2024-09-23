#include "common_test_fixture.h"
#include "types/currency_code.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank_modifier.h"
#include "stubs/currencies_list_file_content_stub.h"

class CurrenciesExchangeRatesDatabankUpdaterTest : public CommonTestFixture
{
public:
    CurrenciesExchangeRatesDatabankUpdaterTest() = default;

protected:
    CurrenciesExchangeRatesDatabank currenciesExchangeRatesDatabank_{CurrenciesListFileContentStub::getValue()};
};

TEST_F(CurrenciesExchangeRatesDatabankUpdaterTest, CurrenciesCount)
{
    CurrenciesExchangeRatesDatabankModifier currenciesExchangeRatesDatabankUpdater;
    EXPECT_EQ(currenciesExchangeRatesDatabank_.getCurrenciesCodes().size(), 3);
}

TEST_F(CurrenciesExchangeRatesDatabankUpdaterTest, AllCombinationsOfExchangeRatesArePresent)
{
    EXPECT_TRUE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("gbp")));
    EXPECT_TRUE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("usd")));

    EXPECT_TRUE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("eur")));
    EXPECT_TRUE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("usd")));

    EXPECT_TRUE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("eur")));
    EXPECT_TRUE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("gbp")));
}

TEST_F(CurrenciesExchangeRatesDatabankUpdaterTest, XToXExchangeRatesAreNotPresent)
{
    EXPECT_FALSE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("eur")));
    EXPECT_FALSE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("gbp")));
    EXPECT_FALSE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("usd")));
}
