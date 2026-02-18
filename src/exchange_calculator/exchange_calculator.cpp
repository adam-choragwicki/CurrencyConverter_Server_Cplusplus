#include "exchange_calculator.h"
#include "types/exchange_rate.h"
#include <iomanip>
#include <sstream>
#include <cmath>
#include "spdlog/spdlog.h"
#include "types/exchange_result.h"

ExchangeResult ExchangeCalculator::convert(const CurrencyAmount& currencyAmount, const ExchangeRate& exchangeRate)
{
    return ExchangeResult(executeCalculation(currencyAmount, exchangeRate));
}

std::string ExchangeCalculator::executeCalculation(const CurrencyAmount& currencyAmount, const ExchangeRate& exchangeRate)
{
    // DISCLAIMER / DESIGN NOTE
    // This project is a simplified, non-commercial, portfolio-grade currency converter.
    // The calculator uses binary floating-point (`long double`) and rounds the final
    // displayed value to 2 decimal places.
    //
    // Why this is acceptable here:
    //  - Many currencies are commonly displayed with 2 decimals.
    //  - The focus of this project is architecture, robustness, and API design,
    //    not finance-grade accounting correctness.
    //
    // Limitations / important caveats:
    //  - Floating-point arithmetic cannot represent most decimal fractions exactly.
    //  - Always rounding to 2 decimals is not correct for all currencies
    //    (some have 0 decimals, some use 3; some have non-decimal minor units).
    long double currencyAmountNumeric = 0.0L;
    long double rateNumeric = 0.0L;
    std::string errorMessage;

    try
    {
        currencyAmountNumeric = std::stold(currencyAmount.toString());
    }
    catch (const std::invalid_argument&)
    {
        errorMessage = std::format("Invalid numeric format for amount: '{}'", currencyAmount.toString());
        throw ConversionError(errorMessage);
    }
    catch (const std::out_of_range&)
    {
        errorMessage = std::format("Amount out of range: '{}'", currencyAmount.toString());
        throw ConversionError(errorMessage);
    }

    try
    {
        rateNumeric = std::stold(exchangeRate.toString());
    }
    catch (const std::invalid_argument&)
    {
        errorMessage = std::format("Invalid numeric format for exchange rate: '{}'", exchangeRate.toString());
        throw ConversionError(errorMessage);
    }
    catch (const std::out_of_range&)
    {
        errorMessage = std::format("Exchange rate out of range: '{}'", exchangeRate.toString());
        throw ConversionError(errorMessage);
    }

    if (rateNumeric <= 0)
    {
        errorMessage = std::format("Non-positive exchange rate provided: '{}'", exchangeRate.toString());
        throw ConversionError(errorMessage);
    }

    const long double rawResult = currencyAmountNumeric * rateNumeric;

    if (!std::isfinite(rawResult))
    {
        errorMessage = "Conversion result out of range";
        throw ConversionError(errorMessage);
    }

    constexpr long double amountScale = 100.0L; // cents
    const long double roundedResult = std::round(rawResult * amountScale) / amountScale;

    if (!std::isfinite(roundedResult))
    {
        errorMessage = "Conversion result out of range";
        throw ConversionError(errorMessage);
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << roundedResult;

    spdlog::debug("Calculation Trace: [Amount: {}] * [Rate: {}] = [Raw: {}] -> [Rounded: {}]",
                  currencyAmountNumeric, rateNumeric, rawResult, oss.str());

    return oss.str();
}
