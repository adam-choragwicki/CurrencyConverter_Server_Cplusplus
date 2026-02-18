#pragma once

#include "abstract_string_wrapper.h"

class ExchangeRate : public AbstractStringWrapper<ExchangeRate>
{
public:
    explicit ExchangeRate(const std::string& string) : AbstractStringWrapper(string)
    {}
};
