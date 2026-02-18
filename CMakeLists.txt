cmake_minimum_required(VERSION 3.16)

project(CurrencyConverterServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_library(project_includes INTERFACE)
target_include_directories(project_includes INTERFACE ${PROJECT_SOURCE_DIR}/src)

add_library(third_party_rapidjson INTERFACE)
target_include_directories(third_party_rapidjson INTERFACE ${PROJECT_SOURCE_DIR}/libs/json)
add_library(third_party::rapidjson ALIAS third_party_rapidjson)

add_library(third_party_httplib INTERFACE)
target_include_directories(third_party_httplib INTERFACE ${PROJECT_SOURCE_DIR}/libs/cpp-httplib)
add_library(third_party::httplib ALIAS third_party_httplib)

add_library(third_party_spdlog INTERFACE)
target_include_directories(third_party_spdlog INTERFACE ${PROJECT_SOURCE_DIR}/libs/spdlog/include)
add_library(third_party::spdlog ALIAS third_party_spdlog)

add_subdirectory(src)
add_subdirectory(tests/unit_tests)
