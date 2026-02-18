#pragma once

#include "abstract_string_wrapper.h"

class CurrenciesNamesAndCodesFileContent : public AbstractStringWrapper<CurrenciesNamesAndCodesFileContent>
{
public:
    explicit CurrenciesNamesAndCodesFileContent(const std::string& string) : AbstractStringWrapper(string)
    {}
};
