#include "application.h"
#include "spdlog/spdlog.h"

int main()
{
    try
    {
        Application application;
    }
    catch (const std::exception& ex)
    {
        spdlog::critical("Unhandled exception: {}", ex.what());
        return 1;
    }
}

// TODO use or remove start_currency_converter_server.sh
