#include "common_test_fixture.h"
#include "types/currency_code.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank_modifier.h"
#include "stubs/currencies_list_file_content_stub.h"

class CurrenciesExchangeRatesDatabankModifierTest : public CommonTestFixture
{
public:
    CurrenciesExchangeRatesDatabankModifierTest() = default;

protected:
    CurrenciesExchangeRatesDatabank currenciesExchangeRatesDatabank_{CurrenciesListFileContentStub::getValue()};
};

TEST_F(CurrenciesExchangeRatesDatabankModifierTest, CurrenciesCount)
{
    CurrenciesExchangeRatesDatabankModifier currenciesExchangeRatesDatabankUpdater;
    EXPECT_EQ(currenciesExchangeRatesDatabank_.getCurrenciesCodes().size(), 3);
}

TEST_F(CurrenciesExchangeRatesDatabankModifierTest, AllCombinationsOfExchangeRatesArePresent)
{
//    CurrenciesExchangeRatesDatabankModifier::modifyCurrenciesExchangeRatesDatabank()

    EXPECT_TRUE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("gbp")));
    EXPECT_TRUE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("usd")));

    EXPECT_TRUE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("eur")));
    EXPECT_TRUE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("usd")));

    EXPECT_TRUE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("eur")));
    EXPECT_TRUE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("gbp")));
}

TEST_F(CurrenciesExchangeRatesDatabankModifierTest, XToXExchangeRatesAreNotPresent)
{
    EXPECT_FALSE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("eur"), CurrencyCode("eur")));
    EXPECT_FALSE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("gbp"), CurrencyCode("gbp")));
    EXPECT_FALSE(currenciesExchangeRatesDatabank_.containsExchangeRateData(CurrencyCode("usd"), CurrencyCode("usd")));
}
