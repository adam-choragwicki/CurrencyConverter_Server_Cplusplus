#include "converter.h"
#include <iomanip>

#include "types/money_amount.h"
#include "types/exchange_rate.h"

std::string Converter::convert(const MoneyAmount& moneyAmount, const ExchangeRate& exchangeRate)
{
    return executeCalculation(moneyAmount, exchangeRate);
}

std::string Converter::executeCalculation(const MoneyAmount& moneyAmount, const ExchangeRate& exchangeRate)
{
    const double moneyAmountNumeric = std::stod(moneyAmount.toString());
    const double exchangeRateNumeric = std::stod(exchangeRate.toString());

    const double result = moneyAmountNumeric * exchangeRateNumeric;

    std::ostringstream oss;
    oss << std::setprecision(2) << std::noshowpoint << std::fixed << result;

    return oss.str();
}
