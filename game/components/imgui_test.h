#ifndef GAME_COMPONENTS_IMGUITEST_H_
#define GAME_COMPONENTS_IMGUITEST_H_

#include "defines.h"

#include <memory>
#include <snowpulse.h>

namespace game {
class GAMEAPI ImGUITest : public snowpulse::Component, public snowpulse::Drawable {
    public:
        static std::shared_ptr<ImGUITest> Create();

        virtual ~ImGUITest();
        // From Updatable
        void Update(float deltaTime) override;
        // From Drawable
        void Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) override;

    protected:
        ImGUITest();
};
}   // namespace snowpulse

#endif
