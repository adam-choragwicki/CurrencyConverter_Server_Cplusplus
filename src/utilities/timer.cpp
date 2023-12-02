#include "timer.h"

Timer::Timer() : start_(std::chrono::steady_clock::now())
{}

std::string Timer::getResult()
{
    auto end = std::chrono::steady_clock::now();
    auto difference = end - start_;

    return std::to_string(std::chrono::duration_cast<std::chrono::seconds>(difference).count()) + " seconds";
}
