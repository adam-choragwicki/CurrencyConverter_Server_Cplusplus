#include <gtest/gtest.h>
#include "types/currency_code.h"

TEST(CurrencyCodeTest, LowercasesInput)
{
    const CurrencyCode currencyCode1("eur");
    EXPECT_EQ(currencyCode1.toString(), "eur");

    const CurrencyCode currencyCode2("EUR");
    EXPECT_EQ(currencyCode2.toString(), "eur");
}

TEST(CurrencyCodeTest, MixedCaseIsAccepted)
{
    const CurrencyCode currencyCode1("eUR");
    EXPECT_EQ(currencyCode1.toString(), "eur");

    const CurrencyCode currencyCode2("Eur");
    EXPECT_EQ(currencyCode2.toString(), "eur");
}

TEST(CurrencyCodeTest, EmptyCurrencyCodeThrows)
{
    EXPECT_THROW(CurrencyCode(""), std::runtime_error);
}

TEST(CurrencyCodeTest, WrongLengthCurrencyCodeThrows)
{
    EXPECT_THROW(CurrencyCode("e"), std::runtime_error);
    EXPECT_THROW(CurrencyCode("eu"), std::runtime_error);
    EXPECT_THROW(CurrencyCode("euro"), std::runtime_error);
}

TEST(CurrencyCodeTest, UppercaseRendering)
{
    const CurrencyCode currencyCode("usd");
    EXPECT_EQ(currencyCode.toUpperCase(), "USD");
}
