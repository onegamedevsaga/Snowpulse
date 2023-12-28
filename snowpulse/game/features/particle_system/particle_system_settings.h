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

        bool LoadFromSPPEFile(std::string filename, PathType pathType = PathType::kRaw);
        bool SaveToSPPEFile(std::string filename);

        BlendMode blendMode = BlendMode::kNormal;
        TextureFiltering textureFiltering = TextureFiltering::kBilinear;
        PathType texturePathType = PathType::kAssets;
        std::string textureFilename;

        ValueMode speedValueMode = ValueMode::kRandomWithin;
        float speedA = 270.0f;
        float speedB = 350.0f;

        Vector2 acceleration = Vector2(0.0f, -200.0f);

        ValueMode lifespanValueMode = ValueMode::kRandomWithin;
        float lifespanA = 0.8f;
        float lifespanB = 1.0f;

        Vector2 scaleStartEnd = Vector2(2.0f, 0.2f);

        float angleStart = 0.0f;
        float angleVelocity = 0.0f;

        Color colorStart = Color::Red();
        Color colorEnd = Color(1.0f, 0.6f, 0.0f, 0.0f);

        EmissionShape emissionShape = EmissionShape::kCircle;
        Vector2 emissionRectSize = Vector2(50.0f, 50.0f);
        float emissionRadius = 20.0f;
        float emissionRate = 40.0f;
        int maxParticleCount = 300;
        ValueMode emissionAngleValueMode = ValueMode::kRandomWithin;
        float emissionAngleA = 80.0f;
        float emissionAngleB = 100.0f;
};
}   // namespace snowpulse
#endif
