#pragma once

#include "abstract_string_wrapper.h"

class MoneyAmount : public AbstractStringWrapper
{
public:
    explicit MoneyAmount(const std::string& string) : AbstractStringWrapper(string)
    {}
};
