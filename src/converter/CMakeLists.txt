set(SOURCE_FILES
        converter.h converter.cpp)

add_library(converter ${SOURCE_FILES})
target_link_libraries(converter PUBLIC types)
