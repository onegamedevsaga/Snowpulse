#ifndef SNOWPULSE_COMMON_BLENDMODE_H_
#define SNOWPULSE_COMMON_BLENDMODE_H_

#include "../defines.h"

namespace snowpulse {
enum class BlendMode {
    kDisabled = -1,
    kNormal = 0,
    kAdditive,
    kMultiply,
    kScreen,
};
}   // namespace snowpulse
#endif
