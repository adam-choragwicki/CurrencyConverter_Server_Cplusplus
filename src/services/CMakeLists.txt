set(SOURCE_FILES
        request_processing_service.cpp request_processing_service.h
        cache_update_state.cpp cache_update_state.h)

add_library(request_processing_service ${SOURCE_FILES})
target_link_libraries(request_processing_service PUBLIC project_includes messages types currencies_exchange_rate_databank downloader PRIVATE message_processing)
