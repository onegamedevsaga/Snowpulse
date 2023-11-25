#ifndef SNOWPULSE_APPLICATION_PLATFORM_H_
#define SNOWPULSE_APPLICATION_PLATFORM_H_

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
    kOpenGLES,
};
}   // namespace snowpulse

#endif
