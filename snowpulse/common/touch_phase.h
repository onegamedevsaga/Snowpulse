#ifndef SNOWPULSE_COMMON_TOUCHPHASE_H_
#define SNOWPULSE_COMMON_TOUCHPHASE_H_

#include "../defines.h"

namespace snowpulse {
enum class TouchPhase {
    kBegan = 0,
    kMoved,
    kEnded,
};
}   // namespace snowpulse
#endif
