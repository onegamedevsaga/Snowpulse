#ifndef SNOWPULSE_APPLICATION_TIME_H_
#define SNOWPULSE_APPLICATION_TIME_H_

#include "../defines.h"

#include <memory>
#include <chrono>

namespace snowpulse {
class SNOWPULSEAPI Timer {
    public:
        Timer();
        ~Timer();

        void Start();
        float GetElapsedInSeconds();

    private:
        std::chrono::high_resolution_clock::time_point startTime_;
};
}   // namespace snowpulse
#endif
