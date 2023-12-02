set(SOURCE_FILES
        message_contract.h
        messages.cpp
        raw_inbound_message.h
        abstract_message.h
        abstract_request.h
        abstract_response.h
        parsed_inbound_message.h

        requests/get_config_request.h
        requests/calculate_exchange_request.h
        requests/update_cache_request.h

        responses/get_config_response.h
        responses/calculate_exchange_response.h
        responses/update_cache_response.h)

add_library(messages ${SOURCE_FILES})
target_include_directories(messages PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
