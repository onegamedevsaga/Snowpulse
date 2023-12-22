#ifndef SNOWPULSE_GAME_FEATURES_PARTICLESYSTEM_PARTICLESYSTEM_H_
#define SNOWPULSE_GAME_FEATURES_PARTICLESYSTEM_PARTICLESYSTEM_H_

#include "../../../defines.h"

#include <memory>
#include <string>
#include <map>
#include <vector>

#include "particle_system_settings.h"
#include "particle_data.h"
#include "../../../common/blend_mode.h"
#include "../../../common/texture_filtering.h"
#include "../../../common/matrix4x4.h"
#include "../../../application/graphics.h"

namespace snowpulse {
class SNOWPULSEAPI ParticleSystem {
    public:
        static std::shared_ptr<ParticleSystem> Create(const ParticleSystemSettings& settings);

        virtual ~ParticleSystem();
        void Update(float deltaTime);
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix, int sortOrder, bool isPremultiplied);
        void Play();

    protected:
        ParticleSystem();
        bool SpawnParticle();

        bool isPlaying_;
        ParticleSystemSettings settings_;
        std::vector<ParticleData> particles_;
        float emissionTime_;
};
}   // namespace snowpulse
#endif
