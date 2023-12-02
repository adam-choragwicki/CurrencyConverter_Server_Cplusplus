cmake_minimum_required(VERSION 3.10)
project(CurrencyConverter_Server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(DATA_FILES_DIR ${PROJECT_ROOT}/data)
set(LOGGING_LIB_SRC_DIR ${PROJECT_ROOT}/libs/logging)
set(JSON_LIB_SRC_DIR ${PROJECT_ROOT}/libs/json)

# All modules have access to logging library headers
include_directories(${LOGGING_LIB_SRC_DIR})

# Custom target to copy config.json to bin directory
add_custom_target(copy_config_json
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/config.json ${PROJECT_ROOT}/bin/config.json
        DEPENDS ${CMAKE_SOURCE_DIR}/config.json)

# Custom target to copy data directory to bin directory
add_custom_target(copy_data_directory
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DATA_FILES_DIR} ${PROJECT_ROOT}/bin/data
        DEPENDS ${DATA_FILES_DIR})

# Function to copy data directory and config.json to the specified output directory
function(target_copy_program_data_to_binaries_directory TARGET_NAME)
    # Copy data directory
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${DATA_FILES_DIR} ${PROJECT_ROOT}/bin/data)

    # Copy config.json file
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/config.json ${PROJECT_ROOT}/bin/config.json)

    # Add dependencies on copy_config_json and copy_data_directory
    add_dependencies(${TARGET_NAME} copy_config_json copy_data_directory)
endfunction()

add_subdirectory(src)
add_subdirectory(tests/unit_tests)
