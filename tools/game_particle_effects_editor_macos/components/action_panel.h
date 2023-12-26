#ifndef GAME_COMPONENTS_ACTIONPANEL_H_
#define GAME_COMPONENTS_ACTIONPANEL_H_

#include "../defines.h"

#include <memory>
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

    private:
        ActionPanel();
};
}   // namespace snowpulse
#endif
