#ifndef SNOWPULSE_GAME_FEATURES_PARTICLESYSTEM_PARTICLESYSTEMSETTINGS_H_
#define SNOWPULSE_GAME_FEATURES_PARTICLESYSTEM_PARTICLESYSTEMSETTINGS_H_

#include "../../../defines.h"

#include <string>

#include "../../../common/vector2.h"
#include "../../../common/color.h"
#include "../../../common/path_type.h"
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

        enum class EmissionShape {
            kPoint = 0,
            kRect,
            kCircle,
        };

        BlendMode blendMode = BlendMode::kAdditive;
        TextureFiltering textureFiltering = TextureFiltering::kBilinear;
        PathType texturePathType = PathType::kAssets;
        std::string textureFilename;

        ValueMode speedValueMode = ValueMode::kSingle;
        float speedA = 100.0f;
        float speedB = 100.0f;

        Vector2 acceleration;

        ValueMode lifespanValueMode = ValueMode::kSingle;
        float lifespanA = 1.0f;
        float lifespanB = 1.0f;

        Vector2 scaleStartEnd = Vector2(1.0f, 1.0f);

        float angleStart = 0.0f;
        float angleVelocity = 0.0f;

        Color colorStart = Color::White();
        Color colorEnd = Color::White();

        EmissionShape emissionShape = EmissionShape::kPoint;
        Vector2 emissionRectSize = Vector2(50.0f, 50.0f);
        float emissionRadius = 50.0f;
        float emissionRate = 5.0f;
        int maxParticleCount = 20;
        ValueMode emissionAngleValueMode = ValueMode::kSingle;
        float emissionAngleA = 0.0f;
        float emissionAngleB = 0.0f;
};
}   // namespace snowpulse
#endif
