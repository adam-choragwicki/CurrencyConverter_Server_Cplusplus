#This is library of simple non-dependent types used throughout object*/

set(SOURCE_FILES
        currency_code.h currency_code.cpp
        currency_exchange_rates_json.h
        exchange_rate.h
        exchange_result.h
        currency_amount.h
        timestamp.h
        definitions.h
        currencies_names_and_codes_file_content.h
        containers/exchange_rate_data.h)

add_library(types ${SOURCE_FILES})
target_link_libraries(types PUBLIC project_includes PRIVATE third_party::spdlog)
