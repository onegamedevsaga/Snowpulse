#ifndef SNOWPULSE_GAME_COMPONENTS_PARTICLEEFFECTSRENDERER_H_
#define SNOWPULSE_GAME_COMPONENTS_PARTICLEEFFECTSRENDERER_H_

#include "sprite_renderer.h"

#include <memory>
#include <string>
#include <vector>

#include "../features/particle_system/particle_system.h"
#include "../features/particle_system/particle_system_settings.h"

namespace snowpulse {
class SNOWPULSEAPI ParticleEffectsRenderer : public SpriteRenderer {
    public:
        static std::shared_ptr<ParticleEffectsRenderer> Create(const ParticleSystemSettings& settings);

        virtual ~ParticleEffectsRenderer();
        // From Updatable
        void Update(float deltaTime) override;
        // From Drawable
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix) override;

    protected:
        ParticleEffectsRenderer();

        void LoadGraphics(Graphics* graphics, const ParticleSystemSettings& settings);

        std::shared_ptr<ParticleSystem> particleSystem_;
};
}   // namespace snowpulse

#endif
