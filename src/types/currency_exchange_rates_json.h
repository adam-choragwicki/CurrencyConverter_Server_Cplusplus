#pragma once

#include "abstract_string_wrapper.h"

class CurrencyExchangeRatesJson : public AbstractStringWrapper
{
public:
    explicit CurrencyExchangeRatesJson(const std::string& string = "NULL") : AbstractStringWrapper(string)
    {}
};
