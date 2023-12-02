set(SOURCE_FILES
        request_parser.h request_parser.cpp
        response_factory.h response_factory.cpp
        request_processor.cpp request_processor.h
        calculate_exchange_request_validator.cpp calculate_exchange_request_validator.h
        inbound_message_queue.h inbound_message_queue.cpp)

add_library(message_processing ${SOURCE_FILES})
target_include_directories(message_processing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(message_processing json_processing)
