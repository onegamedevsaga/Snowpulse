#ifndef SNOWPULSE_COMMON_TEXTUREFILTERING_H_
#define SNOWPULSE_COMMON_TEXTUREFILTERING_H_

#include "../defines.h"

namespace snowpulse {
enum class TextureFiltering {
    kPoint = 0,
    kBilinear,
    kTrilinear,
    kAnisotropic,
};
}   // namespace snowpulse
#endif
