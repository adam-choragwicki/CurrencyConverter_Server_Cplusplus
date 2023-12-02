#pragma once

#include "abstract_string_wrapper.h"

class CurrencyCode : public AbstractStringWrapper
{
public:
    explicit CurrencyCode(const std::string& code);

private:
    std::string toLowerCase(const std::string& string);
};
