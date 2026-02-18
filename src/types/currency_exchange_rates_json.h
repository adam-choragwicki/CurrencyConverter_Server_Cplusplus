#pragma once

#include "abstract_string_wrapper.h"

class CurrencyExchangeRatesJson : public AbstractStringWrapper<CurrencyExchangeRatesJson>
{
public:
    explicit CurrencyExchangeRatesJson(const std::string& string) : AbstractStringWrapper(string)
    {}
};
