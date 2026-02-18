#pragma once

#include "abstract_string_wrapper.h"

class CurrencyCode : public AbstractStringWrapper<CurrencyCode>
{
public:
    explicit CurrencyCode(const std::string& code);

private:
    std::string toLowerCase(const std::string& str);
};
