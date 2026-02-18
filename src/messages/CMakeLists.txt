set(SOURCE_FILES
        message_contract.h
        messages.cpp
        abstract_message.h
        abstract_request.h

        requests/status_request.h
        requests/get_config_request.h
        requests/calculate_exchange_request.h
        requests/update_cache_request.h

        response_dto.h)

add_library(messages ${SOURCE_FILES})
target_link_libraries(messages PUBLIC project_includes types)
