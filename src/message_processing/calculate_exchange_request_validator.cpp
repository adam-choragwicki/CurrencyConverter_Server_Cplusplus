#include "calculate_exchange_request_validator.h"
#include "spdlog/spdlog.h"

ValidationResult CalculateExchangeRequestValidator::validateRequest(const CalculateExchangeRequest& calculateExchangeRequest)
{
    std::string failureReason;

    if(isMoneyAmountEmpty(calculateExchangeRequest))
    {
        failureReason = "Money amount string is empty";
        return {false, failureReason};
    }

    if(!doesMoneyAmountContainOnlyPermittedCharacters(calculateExchangeRequest))
    {
        failureReason = "Money amount string contains forbidden characters";
        return {false, failureReason};
    }

    if(!isMoneyAmountNumeric(calculateExchangeRequest))
    {
        failureReason = "Money amount string is not a number";
        return {false, failureReason};
    }

    if(isMoneyAmountOctalNumber(calculateExchangeRequest))
    {
        failureReason = "Money amount string is octal";
        return {false, failureReason};
    }

    if(isMoneyAmountHexadecimalNumber(calculateExchangeRequest))
    {
        failureReason = "Money amount string is hexadecimal";
        return {false, failureReason};
    }

    if(isMoneyAmountNegativeNumber(calculateExchangeRequest))
    {
        failureReason = "Money amount string is negative number";
        return {false, failureReason};
    }

    if(isSourceCurrencyEqualTargetCurrency(calculateExchangeRequest))
    {
        failureReason = "Source and target currency is the same";
        return {false, failureReason};
    }

    if(!failureReason.empty())
    {
        spdlog::error(failureReason);
    }

    return {true, ""};
}

bool CalculateExchangeRequestValidator::isMoneyAmountEmpty(const CalculateExchangeRequest& calculateExchangeRequest)
{
    const MoneyAmount& moneyAmount = calculateExchangeRequest.getMoneyAmount();
    return moneyAmount.toString().empty();
}

bool CalculateExchangeRequestValidator::doesMoneyAmountContainOnlyPermittedCharacters(const CalculateExchangeRequest& calculateExchangeRequest)
{
    const MoneyAmount& moneyAmount = calculateExchangeRequest.getMoneyAmount();

    return std::ranges::all_of(moneyAmount.toString(), [](const char& character)
    {
        return std::isdigit(character) || character == '.';
    });
}

bool CalculateExchangeRequestValidator::isMoneyAmountNumeric(const CalculateExchangeRequest& calculateExchangeRequest)
{
    const MoneyAmount& moneyAmount = calculateExchangeRequest.getMoneyAmount();

    try
    {
        size_t successfullyProcessedCharacters;
        std::stod(moneyAmount.toString(), &successfullyProcessedCharacters);
        return successfullyProcessedCharacters == moneyAmount.toString().size();
    }
    catch(const std::invalid_argument& exception)
    {
        return false;
    }
}

bool CalculateExchangeRequestValidator::isMoneyAmountOctalNumber(const CalculateExchangeRequest& calculateExchangeRequest)
{
    const MoneyAmount& moneyAmount = calculateExchangeRequest.getMoneyAmount();
    return moneyAmount.toString().length() >= 2 && moneyAmount.toString().starts_with("0") && moneyAmount.toString().at(1) != '.';
}

bool CalculateExchangeRequestValidator::isMoneyAmountHexadecimalNumber(const CalculateExchangeRequest& calculateExchangeRequest)
{
    const MoneyAmount& moneyAmount = calculateExchangeRequest.getMoneyAmount();
    return moneyAmount.toString().starts_with("0x");
}

bool CalculateExchangeRequestValidator::isMoneyAmountNegativeNumber(const CalculateExchangeRequest& calculateExchangeRequest)
{
    const MoneyAmount& moneyAmount = calculateExchangeRequest.getMoneyAmount();
    return moneyAmount.toString().at(0) == '-';
}

bool CalculateExchangeRequestValidator::isSourceCurrencyEqualTargetCurrency(const CalculateExchangeRequest& calculateExchangeRequest)
{
    return calculateExchangeRequest.getSourceCurrencyCode() == calculateExchangeRequest.getTargetCurrencyCode();
}
