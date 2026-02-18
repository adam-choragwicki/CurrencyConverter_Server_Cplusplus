#include <gtest/gtest.h>
#include "downloader/download_report.h"
#include "types/currency_code.h"

TEST(DownloadReportTest, TracksRequestedAndSuccessfulDownloads)
{
    DownloadReport report("tmp");

    report.addCurrencyCodeOfFileRequestedToBeDownloaded(CurrencyCode("usd"));
    report.addCurrencyCodeOfSuccessfullyDownloadedFile(CurrencyCode("usd"));

    EXPECT_EQ(report.getCurrenciesCodesOfFilesRequestedToBeDownloaded().size(), 1u);
    EXPECT_EQ(report.getCurrencyCodesOfSuccessfullyDownloadedFiles().size(), 1u);
}

TEST(DownloadReportTest, DuplicateRequestsThrow)
{
    DownloadReport report("tmp");

    report.addCurrencyCodeOfFileRequestedToBeDownloaded(CurrencyCode("usd"));
    EXPECT_THROW(report.addCurrencyCodeOfFileRequestedToBeDownloaded(CurrencyCode("usd")), std::runtime_error);
}

TEST(DownloadReportTest, DuplicateSuccessThrows)
{
    DownloadReport report("tmp");

    report.addCurrencyCodeOfSuccessfullyDownloadedFile(CurrencyCode("usd"));
    EXPECT_THROW(report.addCurrencyCodeOfSuccessfullyDownloadedFile(CurrencyCode("usd")), std::runtime_error);
}

TEST(DownloadReportTest, AllowsTwoErrorsPerCurrency)
{
    DownloadReport report("tmp");

    report.addDataForFailedDownload(CurrencyCode("usd"), "first");
    report.addDataForFailedDownload(CurrencyCode("usd"), "second");
    EXPECT_EQ(report.getErrorDescriptionsPerCurrencyCode().count(CurrencyCode("usd")), 2u);

    EXPECT_THROW(report.addDataForFailedDownload(CurrencyCode("usd"), "third"), std::runtime_error);
}
