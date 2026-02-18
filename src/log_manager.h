#pragma once

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <filesystem>

class LogManager
{
public:
    static void setupLogging()
    {
        // create sinks: console + file
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        constexpr auto defaultLevel =
#if defined(NDEBUG)
                spdlog::level::info;
#else
        spdlog::level::debug;
#endif
        bool isPytest = false;
#if defined(_WIN32)
        char* pytestValue = nullptr;
        size_t pytestLen = 0;
        if (_dupenv_s(&pytestValue, &pytestLen, "PYTEST_LOGGING") == 0 && pytestValue)
        {
            isPytest = true;
            free(pytestValue);
        }
#else
        isPytest = std::getenv("PYTEST_LOGGING") != nullptr;
#endif
        console_sink->set_level(isPytest ? spdlog::level::warn : defaultLevel);

        std::filesystem::create_directories("logs");
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/server.log", true);
        file_sink->set_level(defaultLevel);

        spdlog::logger logger("", {console_sink, file_sink});
        spdlog::set_default_logger(std::make_shared<spdlog::logger>(logger));
        spdlog::set_level(defaultLevel);
        spdlog::flush_on(spdlog::level::info);
    }
};
