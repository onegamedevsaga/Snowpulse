#ifndef GAME_COMPONENTS_ACTIONPANEL_H_
#define GAME_COMPONENTS_ACTIONPANEL_H_

#include "../defines.h"

#include <memory>
#include <functional>
#include <snowpulse.h>

#include "../models/effect_data.h"

namespace game {
class GAMEAPI ActionPanel : public snowpulse::Component, public snowpulse::Drawable {
    public:
        static std::shared_ptr<ActionPanel> Create();

        virtual ~ActionPanel();
        // From Updatable
        void Update(float deltaTime) override;
        // From Drawable
        void Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) override;

        void SetOnLoadListener(std::function<void(EffectData&)> onLoadFile);
        void CreateNewEffect();

        EffectData& GetEffectData() { return effectData_; }

    private:
        ActionPanel();

        void LoadEffect();
        void SaveEffect();
        void SaveAsEffect();
        void ResetView();
        void ZoomIn();
        void ZoomOut();

        void DrawMenu();

        EffectData effectData_;
        std::function<void(EffectData&)> onLoadFile_;
        bool isFirstTimeDrawingMenu_;
        bool openLoadingInvalidFilePopup_;
};
}   // namespace snowpulse
#endif
