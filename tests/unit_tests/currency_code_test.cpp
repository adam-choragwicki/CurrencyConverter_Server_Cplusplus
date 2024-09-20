#include "common_test_fixture.h"
#include "types/currency_code.h"

class CurrencyCodeTest : public CommonTestFixture
{
};

TEST_F(CurrencyCodeTest, CurrencyCodeIsCaseInsensitive)
{
    CurrencyCode currencyCode1("eur");
    EXPECT_EQ(currencyCode1.toString(), "eur");

    CurrencyCode currencyCode2("EUR");
    EXPECT_EQ(currencyCode2.toString(), "eur");
}

TEST_F(CurrencyCodeTest, MixedCaseCurrencyCodeIsAcceptable)
{
    CurrencyCode currencyCode1("eUR");
    EXPECT_EQ(currencyCode1.toString(), "eur");

    CurrencyCode currencyCode2("Eur");
    EXPECT_EQ(currencyCode1.toString(), "eur");
}

TEST_F(CurrencyCodeTest, EmptyCurrencyCode)
{
    EXPECT_THROW(CurrencyCode(""), std::runtime_error);
}

TEST_F(CurrencyCodeTest, WrongLengthCurrencyCode)
{
    EXPECT_THROW(CurrencyCode("e"), std::runtime_error);
    EXPECT_THROW(CurrencyCode("eu"), std::runtime_error);
    EXPECT_THROW(CurrencyCode("euro"), std::runtime_error);
}

TEST_F(CurrencyCodeTest, UnderlyingStringIsLowerCase)
{
    CurrencyCode currencyCode("USD");

    EXPECT_EQ(currencyCode.toString(), "usd");
}
