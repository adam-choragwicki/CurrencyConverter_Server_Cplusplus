#pragma once

#include "messages/requests/calculate_exchange_request.h"
#include <algorithm>

class ValidationResult
{
public:
    ValidationResult(bool valid, const std::string& failureReason) : valid_(valid), failureReason_(failureReason)
    {}

    [[nodiscard]] bool isValid() const
    { return valid_; }

    [[nodiscard]] const std::string& getFailureReason() const
    { return failureReason_; }

private:
    const bool valid_;
    const std::string failureReason_;
};

class CalculateExchangeRequestValidator
{
public:
    static ValidationResult validateRequest(const CalculateExchangeRequest& calculateExchangeRequest);

    static bool isMoneyAmountEmpty(const CalculateExchangeRequest& calculateExchangeRequest);
    static bool doesMoneyAmountContainOnlyPermittedCharacters(const CalculateExchangeRequest& calculateExchangeRequest);
    static bool isMoneyAmountNumeric(const CalculateExchangeRequest& calculateExchangeRequest);
    static bool isMoneyAmountOctalNumber(const CalculateExchangeRequest& calculateExchangeRequest);
    static bool isMoneyAmountHexadecimalNumber(const CalculateExchangeRequest& calculateExchangeRequest);
    static bool isMoneyAmountNegativeNumber(const CalculateExchangeRequest& calculateExchangeRequest);
    static bool isSourceCurrencyEqualTargetCurrency(const CalculateExchangeRequest& calculateExchangeRequest);
};
