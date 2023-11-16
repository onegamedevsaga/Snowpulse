#include "timer.h"

namespace snowpulse {
Timer::Timer() {
}

Timer::~Timer() {
}

void Timer::Start() {
    startTime_ = std::chrono::high_resolution_clock::now();
}

float Timer::GetElapsedInSeconds() {
    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime_);
    return (float)duration.count();
}
}   // namespace snowpulse
