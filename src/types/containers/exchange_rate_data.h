#pragma once

#include "types/exchange_rate.h"
#include "types/timestamp.h"

class ExchangeRateData
{
public:
    ExchangeRateData(const ExchangeRate& exchangeRate, const Timestamp& timestamp) : exchangeRate_(exchangeRate), timestamp_(timestamp)
    {}

    [[nodiscard]] const ExchangeRate& getExchangeRate() const
    { return exchangeRate_; }

    [[nodiscard]] const Timestamp& getTimestamp() const
    { return timestamp_; }

private:
    ExchangeRate exchangeRate_;
    Timestamp timestamp_;
};
