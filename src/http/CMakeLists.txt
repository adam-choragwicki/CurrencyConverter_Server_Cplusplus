set(SOURCE_FILES
        server_manager.cpp server_manager.h
        correlation_id_http_helper.h
        request_parser.cpp request_parser.h
        request_handlers/abstract_request_handler.cpp request_handlers/abstract_request_handler.h
        request_handlers/status_request_handler.cpp request_handlers/status_request_handler.h
        request_handlers/get_config_request_handler.cpp request_handlers/get_config_request_handler.h
        request_handlers/calculate_exchange_request_handler.cpp request_handlers/calculate_exchange_request_handler.h
        request_handlers/update_cache_request_handler.cpp request_handlers/update_cache_request_handler.h
        request_handlers/update_cache_progress_request_handler.cpp request_handlers/update_cache_progress_request_handler.h)

add_library(http_server ${SOURCE_FILES})
target_link_libraries(http_server
        PUBLIC
        project_includes
        third_party::httplib
        PRIVATE
        messages
        json_processing
        request_processing_service
        types
        third_party::spdlog)
