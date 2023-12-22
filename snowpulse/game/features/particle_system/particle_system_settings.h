#ifndef SNOWPULSE_GAME_FEATURES_PARTICLESYSTEM_PARTICLESYSTEMSETTINGS_H_
#define SNOWPULSE_GAME_FEATURES_PARTICLESYSTEM_PARTICLESYSTEMSETTINGS_H_

#include "../../../defines.h"

#include <string>

#include "../../../common/vector2.h"
#include "../../../common/color.h"
#include "../../../common/blend_mode.h"
#include "../../../common/texture_filtering.h"

namespace snowpulse {
class SNOWPULSEAPI ParticleSystemSettings {
    public:
        enum class ValueMode {
            kSingle = 0,
            kRandomBetween,
            kRandomWithin,
        };

        BlendMode blendMode;
        TextureFiltering textureFiltering = TextureFiltering::kBilinear;
        std::string textureFilename;

        ValueMode directionValueMode = ValueMode::kSingle;
        Vector2 directionA;
        Vector2 directionB;

        ValueMode speedValueMode = ValueMode::kSingle;
        float speedA;
        float speedB;

        Vector2 acceleration;

        ValueMode lifespanValueMode = ValueMode::kSingle;
        float lifespanA;
        float lifespanB;

        float startScale;
        float scaleVelocity;

        float startAngle;
        float angleVelocity;

        Color startColor = Color::White();
        Color endColor = Color::White();

        float emissionRate;
        int maxParticleCount;
};
}   // namespace snowpulse
#endif
