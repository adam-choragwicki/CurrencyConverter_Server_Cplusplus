set(SOURCE_FILES
        currencies_exchange_rate_databank.cpp currencies_exchange_rate_databank.h
        currencies_exchange_rate_databank_updater.cpp currencies_exchange_rate_databank_updater.h
        currencies_exchange_rate_databank_loader.cpp currencies_exchange_rate_databank_loader.h)

add_library(currencies_exchange_rate_databank ${SOURCE_FILES})
target_link_libraries(currencies_exchange_rate_databank downloader types utilities)
