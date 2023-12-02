set(SOURCE_FILES
        download_manager.cpp download_manager.h
        curl_manager.cpp curl_manager.h
        utilities.cpp utilities.h
        download_report.cpp download_report.h)

add_library(downloader ${SOURCE_FILES})
target_link_libraries(downloader PRIVATE curl types)
