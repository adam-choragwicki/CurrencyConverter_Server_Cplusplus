set(SOURCE_FILES
        download_manager.cpp download_manager.h
        downloader.cpp downloader.h
        utilities.cpp utilities.h)

add_library(downloader ${SOURCE_FILES})
target_link_libraries(downloader PRIVATE curl types)
