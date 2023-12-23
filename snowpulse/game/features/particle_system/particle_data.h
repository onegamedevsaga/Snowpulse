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

        float lifespan = 1.0f;
        float currentLife = 1.0f;
        Vector2 position;
        float rotation = 0.0f;
        float scale = 1.0f;
        Color color = Color::White();
        Vector2 size = Vector2(100.0f, 100.0f);

        Vector2 velocity;
};
}   // namespace snowpulse
#endif
