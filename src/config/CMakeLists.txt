set(SOURCE_FILES
        config_loader.h config_loader.cpp
        config.h config.cpp)

add_library(config ${SOURCE_FILES})
target_link_libraries(config json_processing)
