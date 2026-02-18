set(TEST_SOURCES
        main.cpp
        test_helpers.h
        test_currency_code.cpp
        test_converter.cpp
        test_calculate_exchange_request_validator.cpp
        test_http_request_parser.cpp
        test_json_parser.cpp
        test_json_validator.cpp
        test_currencies_exchange_rate_databank.cpp
        test_download_report.cpp
        test_cache_update.cpp)

if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()

add_subdirectory(googletest)

add_executable(currency_converter_server_unit_tests ${TEST_SOURCES})

target_link_libraries(currency_converter_server_unit_tests
        PRIVATE
        project_includes
        third_party::spdlog
        types
        converter
        json_processing
        message_processing
        currencies_exchange_rate_databank
        downloader
        http_server
        gtest)

target_compile_definitions(currency_converter_server_unit_tests
        PRIVATE
        UNIT_TEST_DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\")

if (MSVC)
    target_compile_options(currency_converter_server_unit_tests PRIVATE /FS)
endif ()

include(GoogleTest)

gtest_discover_tests(currency_converter_server_unit_tests)
