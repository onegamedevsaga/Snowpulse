#ifndef GAME_COMPONENTS_ACTIONPANEL_H_
#define GAME_COMPONENTS_ACTIONPANEL_H_

#include "../defines.h"

#include <memory>
#include <functional>
#include <snowpulse.h>

namespace game {
class GAMEAPI ActionPanel : public snowpulse::Component, public snowpulse::Drawable {
    public:
        static std::shared_ptr<ActionPanel> Create();

        virtual ~ActionPanel();
        // From Updatable
        void Update(float deltaTime) override;
        // From Drawable
        void Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) override;

        void SetOnLoadListener(std::function<void(snowpulse::ParticleSystemSettings&)> onLoadFile);
        void CreateNewEffect();
        void CopySettings(snowpulse::ParticleSystemSettings& settings) { settings_ = settings; }

    private:
        ActionPanel();

        void LoadEffect();
        void SaveEffect();
        void SaveAsEffect();
        void ResetView();
        void ZoomIn();
        void ZoomOut();

        void DrawMenu();

        std::function<void(snowpulse::ParticleSystemSettings&)> onLoadFile_;
        snowpulse::ParticleSystemSettings settings_;
        std::string previousFilename_;
        bool isFirstTimeDrawingMenu_;
        bool openLoadingInvalidFilePopup_;
};
}   // namespace snowpulse
#endif
