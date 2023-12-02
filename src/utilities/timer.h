#pragma once

#include <chrono>
#include <string>

class Timer
{
public:
    Timer();
    std::string getResult();

private:
    std::chrono::steady_clock::time_point start_;
};
