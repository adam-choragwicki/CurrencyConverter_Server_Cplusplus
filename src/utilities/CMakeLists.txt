set(SOURCE_FILES
        file_helper.h
        timer.h timer.cpp
        utilities.cpp utilities.h)

add_library(utilities ${SOURCE_FILES})
target_link_libraries(utilities PUBLIC project_includes types third_party::spdlog PRIVATE json_processing)
