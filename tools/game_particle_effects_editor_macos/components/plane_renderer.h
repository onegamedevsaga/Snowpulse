#ifndef GAME_COMPONENTS_PLANERENDERER_H_
#define GAME_COMPONENTS_PLANERENDERER_H_

#include "../defines.h"

#include <memory>
#include <snowpulse.h>

namespace game {
class GAMEAPI PlaneRenderer : public snowpulse::Renderer {
    public:
        static std::shared_ptr<PlaneRenderer> Create();

        virtual ~PlaneRenderer();
        // From Updatable
        void Update(float deltaTime) override;
        // From Drawable
        void Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) override;

    private:
        PlaneRenderer();

        snowpulse::Vector2 GetScreenToResolutionFactor();

        snowpulse::Camera* camera_;
        snowpulse::Input* input_;
        bool isMouseDragging_;
        snowpulse::Vector2 mouseDownPosition_;
        snowpulse::Vector2 cameraPositionOnMouseDown_;
};
}   // namespace snowpulse
#endif
