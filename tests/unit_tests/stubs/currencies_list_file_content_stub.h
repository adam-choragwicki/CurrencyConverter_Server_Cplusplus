#pragma once

#include "types/currencies_list_file_content.h"

class CurrenciesListFileContentStub
{
public:
    static CurrenciesListFileContent getValue()
    {
        CurrenciesListFileContent currenciesListFileContent(R"({
                "Euro": "EUR",
                "U.K. Pound Sterling": "GBP",
                "U.S. Dollar": "USD"
            })");

        return currenciesListFileContent;
    }
};
