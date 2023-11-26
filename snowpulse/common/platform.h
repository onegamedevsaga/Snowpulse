#ifndef SNOWPULSE_COMMON_PLATFORM_H_
#define SNOWPULSE_COMMON_PLATFORM_H_

#include "../defines.h"

namespace snowpulse {
enum class Platform {
    kUnknown = 0,
    kWindows,
    kMacOS,
    kAndroid,
    kIOS,
    kWebGL,
    kOpenGL,
    kMetal,
};
}   // namespace snowpulse

#endif
