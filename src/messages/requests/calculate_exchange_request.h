#pragma once

#include "messages/abstract_request.h"
#include "messages/message_contract.h"
#include "types/currency_code.h"
#include "types/money_amount.h"

class CalculateExchangeRequest : public AbstractRequest
{
public:
    CalculateExchangeRequest(const MessageBody& messageBody, const CorrelationId& correlationId, const CurrencyCode& sourceCurrencyCode, const CurrencyCode& targetCurrencyCode, const MoneyAmount& moneyAmount) :
            AbstractRequest(MessageContract::MessageType::RequestType::CALCULATE_EXCHANGE_REQUEST, messageBody, correlationId), sourceCurrencyCode_(sourceCurrencyCode), targetCurrencyCode_(targetCurrencyCode), moneyAmount_(moneyAmount)
    {}

    [[nodiscard]] const CurrencyCode& getSourceCurrencyCode() const
    {
        return sourceCurrencyCode_;
    }

    [[nodiscard]] const CurrencyCode& getTargetCurrencyCode() const
    {
        return targetCurrencyCode_;
    }

    [[nodiscard]] const MoneyAmount& getMoneyAmount() const
    {
        return moneyAmount_;
    }

    const CurrencyCode sourceCurrencyCode_;
    const CurrencyCode targetCurrencyCode_;
    const MoneyAmount moneyAmount_;
};
