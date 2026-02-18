add_subdirectory(types)
add_subdirectory(json_processing)
add_subdirectory(messages)
add_subdirectory(downloader)
add_subdirectory(message_processing)
add_subdirectory(currencies_exchange_rate_databank)
add_subdirectory(exchange_calculator)
add_subdirectory(utilities)
add_subdirectory(services)
add_subdirectory(http)

add_executable(CurrencyConverterServer
        main.cpp
        application.cpp application.h
        log_manager.h)

target_link_libraries(CurrencyConverterServer
        PRIVATE
        project_includes
        third_party::spdlog
        http_server
        request_processing_service
        json_processing
        utilities
        downloader
        currencies_exchange_rate_databank)

set_target_properties(CurrencyConverterServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
