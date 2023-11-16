#ifndef SNOWPULSE_COMMON_EASING_H_
#define SNOWPULSE_COMMON_EASING_H_

#include "../defines.h"

namespace snowpulse {
enum class Easing {
    kLinear = 0,
    kSineIn,
    kSineOut,
    kSineInOut,
    kQuarticIn,
    kQuarticOut,
    kQuarticInOut,
    kElasticIn,
    kElasticOut,
    kElasticInOut,
    kBounceIn,
    kBounceOut,
    kBounceInOut,
};
}   // namespace snowpulse
#endif
