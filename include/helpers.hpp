#pragma once

#include <chrono>

#include "enums.hpp"

inline TimeStamp getUnixTime() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

void printReport(std::pair<Price, Volume> fill, TimeStamp startTime, TimeStamp endTime);