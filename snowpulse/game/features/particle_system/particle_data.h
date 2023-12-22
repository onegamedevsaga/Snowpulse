#ifndef SNOWPULSE_GAME_FEATURES_PARTICLESYSTEM_PARTICLEDATA_H_
#define SNOWPULSE_GAME_FEATURES_PARTICLESYSTEM_PARTICLEDATA_H_

#include "../../../defines.h"

#include <memory>
#include <string>
#include <map>
#include <vector>

#include "../../../common/color.h"

namespace snowpulse {
class SNOWPULSEAPI ParticleData {
    public:

        float lifespan;
        Vector2 position;
        float rotation;
        float scale;
        Color color;

        Vector2 direction;
        float speed;
};
}   // namespace snowpulse
#endif
