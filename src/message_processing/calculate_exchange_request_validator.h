#pragma once

#include "messages/requests/calculate_exchange_request.h"
#include <optional>
#include <string>

class ValidationResult
{
public:
    static ValidationResult setSuccess()
    {
        return {true, std::nullopt};
    }

    static ValidationResult setFailure(std::string failureReason)
    {
        return {false, std::move(failureReason)};
    }

    [[nodiscard]] bool isSuccess() const { return valid_; }
    [[nodiscard]] const std::optional<std::string>& getFailureReason() const { return failureReason_; }

private:
    ValidationResult(const bool valid, std::optional<std::string> failureReason) : valid_(valid), failureReason_(std::move(failureReason))
    {}

    const bool valid_;
    const std::optional<std::string> failureReason_;
};

class CalculateExchangeRequestValidator
{
public:
    static ValidationResult validateRequest(const CalculateExchangeRequest& calculateExchangeRequest);

    static bool isCurrencyAmountEmpty(const CalculateExchangeRequest& calculateExchangeRequest);
    static bool doesCurrencyAmountContainOnlyPermittedCharacters(const CalculateExchangeRequest& calculateExchangeRequest);
    static bool isCurrencyAmountNumeric(const CalculateExchangeRequest& calculateExchangeRequest);
    static bool isCurrencyAmountOctalNumber(const CalculateExchangeRequest& calculateExchangeRequest);
    static bool isCurrencyAmountHexadecimalNumber(const CalculateExchangeRequest& calculateExchangeRequest);
    static bool isCurrencyAmountNegativeNumber(const CalculateExchangeRequest& calculateExchangeRequest);
    static bool isSourceCurrencyEqualTargetCurrency(const CalculateExchangeRequest& calculateExchangeRequest);
};
