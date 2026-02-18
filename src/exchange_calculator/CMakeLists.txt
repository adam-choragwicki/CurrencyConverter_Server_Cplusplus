set(SOURCE_FILES
        exchange_calculator.cpp exchange_calculator.h)

add_library(converter ${SOURCE_FILES})
target_link_libraries(converter
        PUBLIC
        project_includes
        types
        PRIVATE
        third_party::spdlog)
