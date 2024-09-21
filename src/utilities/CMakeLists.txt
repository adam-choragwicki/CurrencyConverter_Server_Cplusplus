set(SOURCE_FILES
        files_helper.h
        timer.h timer.cpp
        unique_id_generator.h)

add_library(utilities ${SOURCE_FILES})
target_link_libraries(utilities PRIVATE types)
