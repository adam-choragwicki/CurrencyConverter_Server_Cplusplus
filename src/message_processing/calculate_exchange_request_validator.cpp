#include "calculate_exchange_request_validator.h"
#include "spdlog/spdlog.h"

ValidationResult CalculateExchangeRequestValidator::validateRequest(const CalculateExchangeRequest& calculateExchangeRequest)
{
    std::string failureReason;

    if (isCurrencyAmountEmpty(calculateExchangeRequest))
    {
        failureReason = "Money amount string is empty";
        return ValidationResult::setFailure(failureReason);
    }

    if (!doesCurrencyAmountContainOnlyPermittedCharacters(calculateExchangeRequest))
    {
        failureReason = "Money amount string contains forbidden characters";
        return ValidationResult::setFailure(failureReason);
    }

    if (!isCurrencyAmountNumeric(calculateExchangeRequest))
    {
        failureReason = "Money amount string is not a number";
        return ValidationResult::setFailure(failureReason);
    }

    if (isCurrencyAmountOctalNumber(calculateExchangeRequest))
    {
        failureReason = "Money amount string is octal";
        return ValidationResult::setFailure(failureReason);
    }

    if (isCurrencyAmountHexadecimalNumber(calculateExchangeRequest))
    {
        failureReason = "Money amount string is hexadecimal";
        return ValidationResult::setFailure(failureReason);
    }

    if (isCurrencyAmountNegativeNumber(calculateExchangeRequest))
    {
        failureReason = "Money amount string is negative number";
        return ValidationResult::setFailure(failureReason);
    }

    if (isSourceCurrencyEqualTargetCurrency(calculateExchangeRequest))
    {
        failureReason = "Source and target currency is the same";
        return ValidationResult::setFailure(failureReason);
    }

    return ValidationResult::setSuccess();
}

bool CalculateExchangeRequestValidator::isCurrencyAmountEmpty(const CalculateExchangeRequest& calculateExchangeRequest)
{
    const CurrencyAmount& currencyAmount = calculateExchangeRequest.getSourceCurrencyAmount();
    return currencyAmount.toString().empty();
}

bool CalculateExchangeRequestValidator::doesCurrencyAmountContainOnlyPermittedCharacters(const CalculateExchangeRequest& calculateExchangeRequest)
{
    const CurrencyAmount& currencyAmount = calculateExchangeRequest.getSourceCurrencyAmount();

    return std::ranges::all_of(currencyAmount.toString(), [](const char& character)
    {
        return std::isdigit(character) || character == '.';
    });
}

bool CalculateExchangeRequestValidator::isCurrencyAmountNumeric(const CalculateExchangeRequest& calculateExchangeRequest)
{
    const CurrencyAmount& currencyAmount = calculateExchangeRequest.getSourceCurrencyAmount();

    try
    {
        size_t successfullyProcessedCharacters;
        static_cast<void>(std::stod(currencyAmount.toString(), &successfullyProcessedCharacters));
        return successfullyProcessedCharacters == currencyAmount.toString().size();
    }
    catch (const std::invalid_argument&)
    {
        return false;
    }
}

bool CalculateExchangeRequestValidator::isCurrencyAmountOctalNumber(const CalculateExchangeRequest& calculateExchangeRequest)
{
    const CurrencyAmount& currencyAmount = calculateExchangeRequest.getSourceCurrencyAmount();
    return currencyAmount.toString().length() >= 2 && currencyAmount.toString().starts_with("0") && currencyAmount.toString().at(1) != '.';
}

bool CalculateExchangeRequestValidator::isCurrencyAmountHexadecimalNumber(const CalculateExchangeRequest& calculateExchangeRequest)
{
    const CurrencyAmount& currencyAmount = calculateExchangeRequest.getSourceCurrencyAmount();
    return currencyAmount.toString().starts_with("0x");
}

bool CalculateExchangeRequestValidator::isCurrencyAmountNegativeNumber(const CalculateExchangeRequest& calculateExchangeRequest)
{
    const CurrencyAmount& currencyAmount = calculateExchangeRequest.getSourceCurrencyAmount();
    return currencyAmount.toString().at(0) == '-';
}

bool CalculateExchangeRequestValidator::isSourceCurrencyEqualTargetCurrency(const CalculateExchangeRequest& calculateExchangeRequest)
{
    return calculateExchangeRequest.getSourceCurrencyCode() == calculateExchangeRequest.getTargetCurrencyCode();
}
