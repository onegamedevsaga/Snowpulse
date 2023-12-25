#ifndef SNOWPULSE_COMMON_BLENDMODE_H_
#define SNOWPULSE_COMMON_BLENDMODE_H_

#include "../defines.h"

namespace snowpulse {
enum class BlendMode {
    kDisabled = 0,
    kNormal,
    kAdditive,
    kMultiply,
    kScreen,
};
}   // namespace snowpulse
#endif
