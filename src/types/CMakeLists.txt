#This is library of simple non-dependent types used throughout object*/

set(SOURCE_FILES
        currency_code.h currency_code.cpp
        currency_exchange_rates_json.h
        exchange_rate.h
        money_amount.h
        timestamp.h
        message_body.h
        correlation_id.h
        client_id.h
        client_info.h client_info.cpp definitions.h
        containers/exchange_rate_data.h)

add_library(types ${SOURCE_FILES})
target_include_directories(types PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
