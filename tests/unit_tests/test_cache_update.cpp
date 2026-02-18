#include <gtest/gtest.h>

#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank.h"
#include "currencies_exchange_rate_databank/currencies_exchange_rate_databank_update_manager.h"
#include "downloader/download_manager.h"
#include "types/currencies_names_and_codes_file_content.h"
#include "types/currency_code.h"
#include "paths.h"
#include "test_helpers.h"

#include <cstdlib>

namespace
{
    struct ScopedEnvVar
    {
        explicit ScopedEnvVar(const char* key, const std::string& value) : key_(key)
        {
            // We only need "set", not "restore previous", for these tests.
#if defined(_WIN32)
            ::_putenv_s(key, value.c_str());
#else
            ::setenv(key, value.c_str(), 1);
#endif
        }

        ~ScopedEnvVar()
        {
#if defined(_WIN32)
            ::_putenv_s(key_.c_str(), "");
#else
            ::unsetenv(key_.c_str());
#endif
        }

        std::string key_;
    };

    const std::string currenciesListJson = R"({
  "Euro": "EUR",
  "U.S. Dollar": "USD",
  "U.K. Pound Sterling": "GBP",
  "Polish Zloty": "PLN"
})";

    const std::string oldDate = "2024-01-01";
    const std::string newDate = "2025-02-02";

    std::string MakeRatesJsonForBase(const std::string& base,
                                     const std::string& other1,
                                     const std::string& other2,
                                     const std::string& other3,
                                     const double r1,
                                     const double r2,
                                     const double r3,
                                     const std::string& date)
    {
        (void) base;
        std::ostringstream out;
        out << "{\n"
                << "  \"" << other1 << "\": {\"rate\": " << r1 << ", \"date\": \"" << date << "\"},\n"
                << "  \"" << other2 << "\": {\"rate\": " << r2 << ", \"date\": \"" << date << "\"},\n"
                << "  \"" << other3 << "\": {\"rate\": " << r3 << ", \"date\": \"" << date << "\"}\n"
                << "}";
        return out.str();
    }
}

TEST(CacheUpdateTest, PartialUpdateAppliesNewDataAndKeepsOldForMissingCurrencies)
{
    ScopedTempDir tempDir;
    ScopedCurrentPath cwd(tempDir.path());

    const auto cacheDir = tempDir.path() / Paths::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH;

    // Initial cache: all 4 base currencies present, all have "old" timestamps.
    WriteFileToString(cacheDir / "eur.json", MakeRatesJsonForBase("eur", "usd", "gbp", "pln", 1.10, 0.80, 4.00, oldDate));
    WriteFileToString(cacheDir / "usd.json", MakeRatesJsonForBase("usd", "eur", "gbp", "pln", 0.90, 0.70, 3.60, oldDate));
    WriteFileToString(cacheDir / "gbp.json", MakeRatesJsonForBase("gbp", "eur", "usd", "pln", 1.20, 1.30, 5.00, oldDate));
    WriteFileToString(cacheDir / "pln.json", MakeRatesJsonForBase("pln", "eur", "usd", "gbp", 0.25, 0.28, 0.20, oldDate));

    const CurrenciesNamesAndCodesFileContent listContent(currenciesListJson);
    CurrenciesExchangeRatesDatabank databank(listContent);

    const auto beforeGbpToEur = databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("gbp"), CurrencyCode("eur"));
    ASSERT_TRUE(beforeGbpToEur.has_value());
    ASSERT_EQ(beforeGbpToEur->getTimestamp().toString(), oldDate);

    // Local update data: only EUR and USD updated, GBP/PLN missing => partial update.
    const auto localUpdateDir = tempDir.path() / "local_update";
    WriteFileToString(localUpdateDir / "eur.json", MakeRatesJsonForBase("eur", "usd", "gbp", "pln", 1.11, 0.81, 4.01, newDate));
    WriteFileToString(localUpdateDir / "usd.json", MakeRatesJsonForBase("usd", "eur", "gbp", "pln", 0.91, 0.71, 3.61, newDate));

    ScopedEnvVar useLocal("UPDATE_CACHE_USE_LOCAL_DATA", "1");
    ScopedEnvVar localDir("UPDATE_CACHE_LOCAL_DIR", localUpdateDir.string());

    size_t lastCompleted{};
    size_t lastTotal{};
    const auto onProgress = [&](const size_t completed, const size_t total)
    {
        lastCompleted = completed;
        lastTotal = total;
    };

    DownloadManager downloadManager;
    const bool ok = CurrenciesExchangeRatesDatabankUpdateManager::startCurrenciesExchangeRatesDatabankUpdate(databank, downloadManager, onProgress);
    EXPECT_TRUE(ok);
    EXPECT_EQ(lastCompleted, lastTotal);
    EXPECT_EQ(lastTotal, 4u);

    // EUR base should now have a "new" timestamp.
    const auto afterEurToUsd = databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("eur"), CurrencyCode("usd"));
    ASSERT_TRUE(afterEurToUsd.has_value());
    EXPECT_EQ(afterEurToUsd->getTimestamp().toString(), newDate);

    // GBP base was missing in update data, so it should remain "old".
    const auto afterGbpToEur = databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("gbp"), CurrencyCode("eur"));
    EXPECT_EQ(afterGbpToEur->getTimestamp().toString(), oldDate);
}

TEST(CacheUpdateTest, UpdateFailsWhenNothingCouldBeDownloadedOrCopied)
{
    ScopedTempDir tempDir;
    ScopedCurrentPath cwd(tempDir.path());

    const auto cacheDir = tempDir.path() / Paths::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH;

    // The initial cache exists, but the update has no files.
    WriteFileToString(cacheDir / "eur.json", MakeRatesJsonForBase("eur", "usd", "gbp", "pln", 1.10, 0.80, 4.00, oldDate));
    WriteFileToString(cacheDir / "usd.json", MakeRatesJsonForBase("usd", "eur", "gbp", "pln", 0.90, 0.70, 3.60, oldDate));
    WriteFileToString(cacheDir / "gbp.json", MakeRatesJsonForBase("gbp", "eur", "usd", "pln", 1.20, 1.30, 5.00, oldDate));
    WriteFileToString(cacheDir / "pln.json", MakeRatesJsonForBase("pln", "eur", "usd", "gbp", 0.25, 0.28, 0.20, oldDate));

    const CurrenciesNamesAndCodesFileContent listContent(currenciesListJson);
    CurrenciesExchangeRatesDatabank databank(listContent);

    const auto localUpdateDir = tempDir.path() / "local_update_empty";
    std::filesystem::create_directories(localUpdateDir);

    ScopedEnvVar useLocal("UPDATE_CACHE_USE_LOCAL_DATA", "1");
    ScopedEnvVar localDir("UPDATE_CACHE_LOCAL_DIR", localUpdateDir.string());

    size_t lastCompleted{};
    size_t lastTotal{};
    const auto onProgress = [&](const size_t completed, const size_t total)
    {
        lastCompleted = completed;
        lastTotal = total;
    };

    DownloadManager downloadManager;
    const bool ok = CurrenciesExchangeRatesDatabankUpdateManager::startCurrenciesExchangeRatesDatabankUpdate(databank, downloadManager, onProgress);
    EXPECT_FALSE(ok);
    EXPECT_EQ(lastCompleted, lastTotal);
    EXPECT_EQ(lastTotal, 4u);

    // Data should remain intact (old timestamp).
    const auto eurToUsd = databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("eur"), CurrencyCode("usd"));
    ASSERT_TRUE(eurToUsd.has_value());
    EXPECT_EQ(eurToUsd->getTimestamp().toString(), oldDate);
}

TEST(CacheUpdateTest, IncompleteUpdateJsonDoesNotPoisonExistingRates)
{
    ScopedTempDir tempDir;
    ScopedCurrentPath cwd(tempDir.path());

    const auto cacheDir = tempDir.path() / Paths::CURRENCIES_EXCHANGE_RATE_CACHE_DIRECTORY_PATH;

    WriteFileToString(cacheDir / "eur.json", MakeRatesJsonForBase("eur", "usd", "gbp", "pln", 1.10, 0.80, 4.00, oldDate));
    WriteFileToString(cacheDir / "usd.json", MakeRatesJsonForBase("usd", "eur", "gbp", "pln", 0.90, 0.70, 3.60, oldDate));
    WriteFileToString(cacheDir / "gbp.json", MakeRatesJsonForBase("gbp", "eur", "usd", "pln", 1.20, 1.30, 5.00, oldDate));
    WriteFileToString(cacheDir / "pln.json", MakeRatesJsonForBase("pln", "eur", "usd", "gbp", 0.25, 0.28, 0.20, oldDate));

    const CurrenciesNamesAndCodesFileContent listContent(currenciesListJson);
    CurrenciesExchangeRatesDatabank databank(listContent);

    // Update data contains EUR file, but it's incomplete (missing "date") and must be ignored.
    const auto localUpdateDir = tempDir.path() / "local_update_incomplete";
    const std::string incompleteEurJson = R"({
  "usd": {"rate": 1.11},
  "gbp": {"rate": 0.81, "date": "2025-02-02"},
  "pln": {"rate": 4.01, "date": "2025-02-02"}
})";

    WriteFileToString(localUpdateDir / "eur.json", incompleteEurJson);
    WriteFileToString(localUpdateDir / "usd.json", MakeRatesJsonForBase("usd", "eur", "gbp", "pln", 0.91, 0.71, 3.61, newDate));

    ScopedEnvVar useLocal("UPDATE_CACHE_USE_LOCAL_DATA", "1");
    ScopedEnvVar localDir("UPDATE_CACHE_LOCAL_DIR", localUpdateDir.string());

    DownloadManager downloadManager;
    const bool ok = CurrenciesExchangeRatesDatabankUpdateManager::startCurrenciesExchangeRatesDatabankUpdate(databank,
                                                                                                             downloadManager,
                                                                                                             [](size_t, size_t) {});
    EXPECT_TRUE(ok); // USD file is valid, so overall update is still OK.

    // EUR base should remain old (EUR file ignored).
    const auto eurToUsd = databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("eur"), CurrencyCode("usd"));
    ASSERT_TRUE(eurToUsd.has_value());
    EXPECT_EQ(eurToUsd->getTimestamp().toString(), oldDate);

    // USD base should be updated.
    const auto usdToEur = databank.tryGetExchangeRateDataForCurrenciesPair(CurrencyCode("usd"), CurrencyCode("eur"));
    ASSERT_TRUE(usdToEur.has_value());
    EXPECT_EQ(usdToEur->getTimestamp().toString(), newDate);
}
