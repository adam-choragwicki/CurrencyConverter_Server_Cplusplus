include(FetchContent)

# --- DISABLE SOME CURL FEATURES TO REMOVE UNNECESSARY EXTERNAL DEPENDENCIES ---
set(HTTP_ONLY ON CACHE INTERNAL "Enable only HTTP/HTTPS protocol")
set(BUILD_CURL_EXE OFF CACHE INTERNAL "No need for curl executable")
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build curl as static lib")
set(CURL_USE_LIBPSL OFF CACHE INTERNAL "Disable PSL")
set(CURL_USE_BROTLI OFF CACHE INTERNAL "Disable Brotli")
set(CURL_USE_ZLIB OFF CACHE INTERNAL "Disable ZLIB")
set(CURL_ENABLE_SSL ON CACHE INTERNAL "Keep SSL for HTTPS") # needed for https://
set(CURL_USE_SCHANNEL ON CACHE INTERNAL "Use Windows native SSL/TLS (Schannel)")
set(CURL_WINDOWS_SSPI ON CACHE INTERNAL "Enable SSPI for Schannel")
set(ENABLE_CURL_MANUAL OFF CACHE INTERNAL "Disable manual")
set(BUILD_LIBCURL_DOCS OFF CACHE INTERNAL "Do not build libcurl docs")

FetchContent_Declare(
        curl
        GIT_REPOSITORY https://github.com/curl/curl.git
        GIT_TAG curl-8_18_0
)

FetchContent_MakeAvailable(curl)

set(SOURCE_FILES
        download_manager.cpp download_manager.h
        curl_manager.cpp curl_manager.h
        utilities.cpp utilities.h
        download_report.cpp download_report.h)

add_library(downloader ${SOURCE_FILES})
target_link_libraries(downloader
        PUBLIC
        project_includes
        types
        PRIVATE
        CURL::libcurl
        third_party::spdlog)
