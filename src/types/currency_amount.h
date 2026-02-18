#pragma once

#include "abstract_string_wrapper.h"

class CurrencyAmount : public AbstractStringWrapper<CurrencyAmount>
{
public:
    explicit CurrencyAmount(const std::string& string) : AbstractStringWrapper(string)
    {}
};
