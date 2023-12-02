cmake_minimum_required(VERSION 3.10)

set(TEST_SOURCES
        main.cpp
        common_test_fixture.h
        test_utilities.h
        test_utilities.cpp
        mocks/client_fake.h
        mocks/server_fake.h
        stubs/config_stub.h
        stubs/currencies_databank_stub.h
        stubs/response_stub.h
        stubs/client_id_stub.h
        request_validator_test.cpp
        converter_test.cpp
        currency_code_test.cpp
        currencies_exchange_rate_databank_test.cpp
        client_socket_handler_test.cpp
        request_parser_test.cpp)

add_subdirectory(libs/googletest)

add_executable(${PROJECT_NAME}Test_run ${TEST_SOURCES})
target_include_directories(${PROJECT_NAME}Test_run PUBLIC src)
target_link_libraries(${PROJECT_NAME}Test_run PUBLIC gtest gmock config converter message_processing socket_communication messages types currencies_exchange_rate_databank)

target_copy_program_data_to_binaries_directory(${PROJECT_NAME}Test_run)

set_target_properties(${PROJECT_NAME}Test_run PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT}/bin)
