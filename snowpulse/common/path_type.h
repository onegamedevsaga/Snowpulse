#ifndef SNOWPULSE_COMMON_PATHTYPE_H_
#define SNOWPULSE_COMMON_PATHTYPE_H_

#include "../defines.h"

namespace snowpulse {
enum class PathType {
    kAssets = 0,
    kDefaults,
    kApplicationSupport,
    kDocuments,
    kRaw,
};
}   // namespace snowpulse
#endif
