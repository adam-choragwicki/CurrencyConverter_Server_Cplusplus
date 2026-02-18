#pragma once

#include "messages/abstract_request.h"
#include "types/currency_code.h"
#include "types/currency_amount.h"

class CalculateExchangeRequest : public AbstractRequest
{
public:
    explicit CalculateExchangeRequest(const CorrelationId& correlationId, const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode, const CurrencyAmount& currencyAmount)
        : AbstractRequest(correlationId), sourceCurrencyCode_(sourceCurrencyCode), targetCurrencyCode_(targetCurrencyCode), sourceCurrencyAmount_(currencyAmount)
    {}

    [[nodiscard]] const CurrencyCode& getSourceCurrencyCode() const { return sourceCurrencyCode_; }
    [[nodiscard]] const CurrencyCode& getTargetCurrencyCode() const { return targetCurrencyCode_; }
    [[nodiscard]] const CurrencyAmount& getSourceCurrencyAmount() const { return sourceCurrencyAmount_; }

private:
    const CurrencyCode sourceCurrencyCode_;
    const CurrencyCode targetCurrencyCode_;
    const CurrencyAmount sourceCurrencyAmount_;
};
