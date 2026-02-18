#pragma once

#include "messages/response_dto.h"
#include <mutex>
#include <string>

class CacheUpdateState
{
public:
    CacheUpdateState() = default;

    CacheUpdateState(const CacheUpdateState&) = delete;
    CacheUpdateState(CacheUpdateState&&) = delete;
    CacheUpdateState& operator=(const CacheUpdateState&) = delete;
    CacheUpdateState& operator=(CacheUpdateState&&) = delete;

    [[nodiscard]] bool tryBegin(size_t total);
    void reportCompleted(size_t completed);
    void finishOk();
    void finishFail(const std::string& error);

    [[nodiscard]] bool isInProgress() const;
    [[nodiscard]] UpdateCacheProgressResponseDto snapshot() const;

private:
    enum class Status
    {
        Idle,
        InProgress,
        Ok,
        Fail
    };

    mutable std::mutex mutex_;
    Status status_{Status::Idle};
    size_t completed_{0};
    size_t total_{0};
    std::string error_;
};

