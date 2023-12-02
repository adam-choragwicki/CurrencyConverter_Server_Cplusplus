#include "application.h"
#include "config/config_loader.h"
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "spdlog/spdlog.h"

void setupLogging(const Config& config);

int main()
{
    Config config = ConfigLoader::readConfigFile("config.json");

    setupLogging(config);

    spdlog::info("Started currency converter server");

    Application application(config);

    spdlog::info("Shutting down currency converter server");

    return 0;
}

void setupLogging(const Config& config)
{
    if(config.isDebug())
    {
        spdlog::set_level(spdlog::level::debug);
    }
    else
    {
        spdlog::set_level(spdlog::level::info);
    }

    /*Add file logging sink*/
    spdlog::default_logger()->sinks().push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/log.txt", true));
    spdlog::flush_on(spdlog::level::debug);
}

//TODO for release mode turn off debug messages logging
