set(SOURCE_FILES
        json_parser.h json_parser.cpp
        json_reader.h json_reader.cpp
        json_writer.h json_writer.cpp
        json_processing_config.h
        exceptions.h)

add_library(json_processing ${SOURCE_FILES})
target_include_directories(json_processing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. ${JSON_LIB_SRC_DIR})
