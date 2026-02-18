set(SOURCE_FILES
        response_factory.h response_factory.cpp
        request_processor.cpp request_processor.h
        calculate_exchange_request_validator.cpp calculate_exchange_request_validator.h)

add_library(message_processing ${SOURCE_FILES})
target_link_libraries(message_processing
        PUBLIC
        project_includes
        messages
        types
        PRIVATE
        converter
        currencies_exchange_rate_databank
        downloader
        json_processing
        third_party::spdlog)
