set(SOURCE_FILES
        json_parser.h json_parser.cpp
        json_reader.h json_reader.cpp
        json_writer.h json_writer.cpp
        json_processing_config.h
        json_validator.cpp json_validator.h
        exceptions.h)

add_library(json_processing ${SOURCE_FILES})
target_link_libraries(json_processing
        PUBLIC
        project_includes
        types
        third_party::rapidjson
        PRIVATE
        third_party::spdlog)
