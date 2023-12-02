#pragma once

#include "abstract_string_wrapper.h"

class CurrenciesListFileContent : public AbstractStringWrapper
{
public:
    explicit CurrenciesListFileContent(const std::string& string = "NULL") : AbstractStringWrapper(string)
    {}
};
