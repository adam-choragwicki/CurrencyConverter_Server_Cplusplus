#pragma once

#include "abstract_string_wrapper.h"

class ExchangeResult : public AbstractStringWrapper<ExchangeResult>
{
public:
    explicit ExchangeResult(const std::string& string) : AbstractStringWrapper(string)
    {}
};
