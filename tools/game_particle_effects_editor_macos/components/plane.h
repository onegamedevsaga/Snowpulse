#ifndef GAME_COMPONENTS_PLANE_H_
#define GAME_COMPONENTS_PLANE_H_

#include "../defines.h"

#include <memory>
#include <snowpulse.h>

namespace game {
class GAMEAPI Plane : public snowpulse::Component, public snowpulse::Drawable {
    public:
        static std::shared_ptr<Plane> Create();

        virtual ~Plane();
        // From Updatable
        void Update(float deltaTime) override;
        // From Drawable
        void Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) override;

    private:
        Plane();

        snowpulse::Camera* camera_;
        snowpulse::Input* input_;
        bool isMouseDragging_;
        snowpulse::Vector2Int resolutionSize_;
        snowpulse::Vector2 screenToResolutionFactor_;
        snowpulse::Vector2 defaultCameraPosition_;
        snowpulse::Vector2 mouseDownPosition_;
        snowpulse::Vector2 cameraPositionOnMouseDown_;
};
}   // namespace snowpulse
#endif
