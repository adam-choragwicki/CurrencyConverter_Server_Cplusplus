#include "test_utilities.h"

void delayMs(int delayMs)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
}

void multithreadingDelayMs()
{
    delayMs(20);
}
