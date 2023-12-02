set(SOURCE_FILES
        main.cpp
        application.cpp application.h)

add_subdirectory(config)
add_subdirectory(types)
add_subdirectory(json_processing)
add_subdirectory(downloader)
add_subdirectory(messages)
add_subdirectory(message_processing)
add_subdirectory(currencies_exchange_rate_databank)
add_subdirectory(converter)
add_subdirectory(socket_communication)
add_subdirectory(utilities)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} config socket_communication message_processing messages types currencies_exchange_rate_databank converter)

target_copy_program_data_to_binaries_directory(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT}/bin)
