#include <gtest/gtest.h>
#include "message_processing/calculate_exchange_request_validator.h"
#include "messages/requests/calculate_exchange_request.h"
#include "types/correlation_id.h"

namespace
{
    CalculateExchangeRequest MakeRequest(const std::string& amount,
                                         const std::string& source = "eur",
                                         const std::string& target = "usd")
    {
        return CalculateExchangeRequest(CorrelationId("cid"),
                                        CurrencyCode(source),
                                        CurrencyCode(target),
                                        CurrencyAmount(amount));
    }
}

TEST(CalculateExchangeRequestValidatorTest, ValidAmountsAreAccepted)
{
    EXPECT_TRUE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("1.50")).isSuccess());
    EXPECT_TRUE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("0.50")).isSuccess());
    EXPECT_TRUE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("0.005")).isSuccess());
    EXPECT_TRUE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("0.00000005")).isSuccess());
    EXPECT_TRUE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("0.")).isSuccess());
}

TEST(CalculateExchangeRequestValidatorTest, RejectsNegativeAmounts)
{
    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("-1.50")).isSuccess());
}

TEST(CalculateExchangeRequestValidatorTest, RejectsOctalAndHexAmounts)
{
    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("0123")).isSuccess());
    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("00123")).isSuccess());
    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("0x123")).isSuccess());
}

TEST(CalculateExchangeRequestValidatorTest, RejectsNonNumericAmounts)
{
    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("abc")).isSuccess());
    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("12.3.4")).isSuccess());
    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("1e2")).isSuccess());
}

TEST(CalculateExchangeRequestValidatorTest, RejectsWhitespaceInAmounts)
{
    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("123 456")).isSuccess());
    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("0. 123")).isSuccess());
}

TEST(CalculateExchangeRequestValidatorTest, RejectsSameCurrencyPairs)
{
    EXPECT_FALSE(CalculateExchangeRequestValidator::validateRequest(MakeRequest("1", "eur", "eur")).isSuccess());
}
