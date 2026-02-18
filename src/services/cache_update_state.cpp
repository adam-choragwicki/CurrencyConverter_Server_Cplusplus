#include "cache_update_state.h"
#include "messages/message_contract.h"

bool CacheUpdateState::tryBegin(const size_t total)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (status_ == Status::InProgress)
    {
        return false;
    }

    status_ = Status::InProgress;
    completed_ = 0;
    total_ = total;
    error_.clear();
    return true;
}

void CacheUpdateState::reportCompleted(const size_t completed)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (status_ != Status::InProgress)
    {
        return;
    }

    completed_ = completed;
    if (completed_ > total_)
    {
        completed_ = total_;
    }
}

void CacheUpdateState::finishOk()
{
    std::lock_guard<std::mutex> lock(mutex_);
    status_ = Status::Ok;
    completed_ = total_;
    error_.clear();
}

void CacheUpdateState::finishFail(const std::string& error)
{
    std::lock_guard<std::mutex> lock(mutex_);
    status_ = Status::Fail;
    error_ = error;
}

bool CacheUpdateState::isInProgress() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return status_ == Status::InProgress;
}

UpdateCacheProgressResponseDto CacheUpdateState::snapshot() const
{
    std::lock_guard<std::mutex> lock(mutex_);

    UpdateCacheProgressResponseDto dto;

    switch (status_)
    {
        case Status::Idle:
            dto.status = MessageContract::MessagePayload::UpdateCacheProgressResponseContract::IDLE_STATUS;
            break;
        case Status::InProgress:
            dto.status = MessageContract::MessagePayload::UpdateCacheProgressResponseContract::IN_PROGRESS_STATUS;
            break;
        case Status::Ok:
            dto.status = MessageContract::MessagePayload::UpdateCacheProgressResponseContract::OK_STATUS;
            break;
        case Status::Fail:
            dto.status = MessageContract::MessagePayload::UpdateCacheProgressResponseContract::FAIL_STATUS;
            break;
    }

    dto.completed = std::to_string(completed_);
    dto.total = std::to_string(total_);
    dto.error = error_;

    return dto;
}

