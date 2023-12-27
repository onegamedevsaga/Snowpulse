#ifndef GAME_SCENEMAIN_H_
#define GAME_SCENEMAIN_H_

#include "defines.h"
#include <snowpulse.h>

#include <memory>

#include "action_panel.h"
#include "inspector_panel.h"

namespace game {
class GAMEAPI SceneMain : public snowpulse::Scene {
    public:
        static std::shared_ptr<SceneMain> Create(snowpulse::Application* application);

        virtual ~SceneMain();
        void Initialize(snowpulse::Application* application);
        void Shutdown();
        void Start();
        void Update(float deltaTime);

    private:
        SceneMain();

        void OnInspectorInvalidate(snowpulse::ParticleSystemSettings settings);
        void OnActionLoadFile(EffectData& effectData);

        float timeLapsed_;
        std::shared_ptr<snowpulse::SpriteRenderer> background_;
        std::shared_ptr<snowpulse::FontRenderer> statsRenderer_;
        std::shared_ptr<snowpulse::ParticleEffectsRenderer> effectRenderer_;
        std::shared_ptr<ActionPanel> actionPanel_;
        std::shared_ptr<InspectorPanel> inspectorPanel_;

        snowpulse::Json currentData_;

};
}

#endif
