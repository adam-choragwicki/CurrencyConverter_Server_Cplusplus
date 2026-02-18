#include <gtest/gtest.h>
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "types/currencies_names_and_codes_file_content.h"
#include "types/currency_code.h"
#include "types/exchange_rate.h"
#include "types/timestamp.h"
#include "test_helpers.h"
#include "paths.h"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

namespace
{
    std::string kCurrenciesListJson = R"({
  "Euro": "EUR",
  "U.S. Dollar": "USD",
  "Polish Zloty": "PLN"
})";

    std::string eurRatesJson = R"({
  "usd": {"rate": 1.1, "date": "2024-01-01"},
  "pln": {"rate": 4.0, "date": "2024-01-01"}
})";

    std::string usdRatesJson = R"({
  "eur": {"rate": 0.9, "date": "2024-01-01"},
  "pln": {"rate": 3.6, "date": "2024-01-01"}
})";

    std::string plnRatesJson = R"({
  "eur": {"rate": 0.25, "date": "2024-01-01"},
  "usd": {"rate": 0.28, "date": "2024-01-01"}
})";

    std::string oldDate = "2024-01-01";
    std::string newDate = "2025-02-02";
}

TEST(CurrenciesExchangeRatesDatabankTest, LoadsExchangeRatesFromCacheFiles)
{
    ScopedTempDir tempDir;
    ScopedCurrentPath cwd(tempDir.path());

    const std::filesystem::path cacheDir = tempDir.path() / Paths::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH;

    WriteFileToString(cacheDir / "eur.json", eurRatesJson);
    WriteFileToString(cacheDir / "usd.json", usdRatesJson);
    WriteFileToString(cacheDir / "pln.json", plnRatesJson);

    const CurrenciesNamesAndCodesFileContent fileContent(kCurrenciesListJson);
    CurrenciesExchangeRatesDatabank databank(fileContent);

    EXPECT_EQ(databank.getCurrenciesCodes().size(), 3u);

    EXPECT_TRUE(databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("eur"), CurrencyCode("usd")).has_value());
    EXPECT_TRUE(databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("eur"), CurrencyCode("pln")).has_value());
    EXPECT_TRUE(databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("usd"), CurrencyCode("eur")).has_value());
    EXPECT_TRUE(databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("usd"), CurrencyCode("pln")).has_value());
    EXPECT_TRUE(databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("pln"), CurrencyCode("eur")).has_value());
    EXPECT_TRUE(databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("pln"), CurrencyCode("usd")).has_value());

    EXPECT_FALSE(databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("eur"), CurrencyCode("eur")).has_value());
    EXPECT_FALSE(databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("usd"), CurrencyCode("usd")).has_value());
    EXPECT_FALSE(databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("pln"), CurrencyCode("pln")).has_value());

    const auto exchangeRateData = databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("eur"), CurrencyCode("usd"));
    ASSERT_TRUE(exchangeRateData.has_value());
    EXPECT_NEAR(std::stod(exchangeRateData->getExchangeRate().toString()), 1.1, 1e-9);
    EXPECT_EQ(exchangeRateData->getTimestamp().toString(), "2024-01-01");
}

TEST(CurrenciesExchangeRatesDatabankTest, ConcurrentReadsAndSwapsRemainConsistent)
{
    ScopedTempDir tempDir;
    ScopedCurrentPath cwd(tempDir.path());

    const std::filesystem::path cacheDir = tempDir.path() / Paths::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH;

    WriteFileToString(cacheDir / "eur.json", eurRatesJson);
    WriteFileToString(cacheDir / "usd.json", usdRatesJson);
    WriteFileToString(cacheDir / "pln.json", plnRatesJson);

    const CurrenciesNamesAndCodesFileContent fileContent(kCurrenciesListJson);
    CurrenciesExchangeRatesDatabank databank(fileContent);

    const auto oldCache = databank.snapshotExchangeRatesCache();
    auto newCache = oldCache;
    newCache[CurrencyCode("eur")].insert_or_assign(CurrencyCode("usd"), ExchangeRateData(ExchangeRate("1.2"), Timestamp(newDate)));

    std::mutex startMutex;
    std::condition_variable startCVar;
    bool start{false};
    std::atomic bad{false};

    constexpr int kIterations = 2000;
    constexpr int kReaderThreads = 4;

    auto waitForStart = [&]()
    {
        std::unique_lock<std::mutex> lock(startMutex);
        startCVar.wait(lock, [&] { return start; });
    };

    auto readerTask = [&]()
    {
        waitForStart();
        for (int i = 0; i < kIterations; ++i)
        {
            try
            {
                const auto data = databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("eur"), CurrencyCode("usd"));

                if (!data.has_value())
                {
                    bad = true;
                    break;
                }

                const std::string timestamp = data->getTimestamp().toString();

                if (timestamp != oldDate && timestamp != newDate)
                {
                    bad = true;
                    break;
                }

                if (!databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("usd"), CurrencyCode("eur")))
                {
                    bad = true;
                    break;
                }
            }
            catch (...)
            {
                bad = true;
                break;
            }
        }
    };

    auto writerTask = [&]()
    {
        waitForStart();
        for (int i = 0; i < kIterations; ++i)
        {
            databank.replaceExchangeRatesCache(newCache);
            databank.replaceExchangeRatesCache(oldCache);
        }
    };

    std::vector<std::thread> threads;
    threads.emplace_back(writerTask);
    for (int i = 0; i < kReaderThreads; ++i)
    {
        threads.emplace_back(readerTask);
    }

    {
        std::lock_guard<std::mutex> lock(startMutex);
        start = true;
    }

    startCVar.notify_all();

    for (auto& thread: threads)
    {
        thread.join();
    }

    EXPECT_FALSE(bad.load());
}
