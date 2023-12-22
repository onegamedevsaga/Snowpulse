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

        BlendMode blendMode = BlendMode::kAdditive;
        TextureFiltering textureFiltering = TextureFiltering::kBilinear;
        std::string textureFilename;

        ValueMode directionValueMode = ValueMode::kSingle;
        Vector2 directionA = Vector2(0.7f, 0.7f);
        Vector2 directionB = Vector2(0.7f, 0.7f);

        ValueMode speedValueMode = ValueMode::kSingle;
        float speedA = 100.0f;
        float speedB = 100.0f;

        Vector2 acceleration;

        ValueMode lifespanValueMode = ValueMode::kSingle;
        float lifespanA = 1.0f;
        float lifespanB = 1.0f;

        float startScale = 1.0f;
        float scaleVelocity = 0.0f;

        float startAngle = 0.0f;
        float angleVelocity = 0.0f;

        Color startColor = Color::White();
        Color endColor = Color::White();

        float emissionRate = 5.0f;
        int maxParticleCount = 20.0f;
};
}   // namespace snowpulse
#endif
