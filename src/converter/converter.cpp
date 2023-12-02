#include "converter.h"
#include <iomanip>

std::string Converter::convert(const MoneyAmount& moneyAmount, const ExchangeRate& exchangeRate)
{
    return executeCalculation(moneyAmount, exchangeRate);
}

std::string Converter::executeCalculation(const MoneyAmount& moneyAmount, const ExchangeRate& exchangeRate)
{
    double moneyAmountNumeric = std::stod(moneyAmount.toString());
    double exchangeRateNumeric = std::stod(exchangeRate.toString());

    double result = exchangeRateNumeric * moneyAmountNumeric;

    std::ostringstream oss;
    oss << std::setprecision(2) << std::noshowpoint << std::fixed << result;

    std::string resultString = oss.str();

    return resultString;
}
