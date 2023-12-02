#pragma once

#include "abstract_string_wrapper.h"

class ExchangeRate : public AbstractStringWrapper
{
public:
    explicit ExchangeRate(const std::string& string) : AbstractStringWrapper(string)
    {}
};
