#ifndef SNOWPULSE_GAME_COMPONENTS_TRANSFORM_H_
#define SNOWPULSE_GAME_COMPONENTS_TRANSFORM_H_

#include "../../defines.h"
#include "../component.h"

#include <memory>
#include <functional>

#include "../../common/easing.h"
#include "../../common/vector2.h"
#include "../../common/vector3.h"
#include "../../common/matrix4x4.h"

namespace snowpulse {

// Forward declaration
class GameObject;
class Graphics;

class SNOWPULSEAPI Transform : public Component {
    public:
        static std::shared_ptr<Transform> Create();

        virtual ~Transform();
        // From Updatable
        void Update(float deltaTime);

        Vector2 GetPosition() { return position_; }
        Vector3 GetPositionVector3() { return Vector3(position_.x, position_.y, 0.0f); }
        Vector2 GetLocalPosition() { return localPosition_; }
        Vector3 GetLocalPositionVector3() { return Vector3(localPosition_.x, localPosition_.y, 0.0f); }

        float GetRotation() { return rotation_; }
        Vector3 GetRotationVector3() { return Vector3(0.0f, 0.0f, rotation_); }
        float GetLocalRotation() { return localRotation_; }
        Vector3 GetLocalRotationVector3() { return Vector3(0.0f, 0.0f, localRotation_); }

        Vector2 GetLocalScale() { return localScale_; }
        Vector3 GetLocalScaleVector3() { return Vector3(localScale_.x, localScale_.y, 1.0f); }

        Matrix4x4 GetLocalMatrix(GameObject* gameObject = SPNULL);
        Matrix4x4 GetParentWorldMatrix();
        float GetParentWorldRotation();

        void SetPosition(Vector2 position);
        void SetLocalPosition(Vector2 position);

        void SetRotation(float rotation);
        void SetLocalRotation(float rotation);

        void SetLocalScale(Vector2 scale) { localScale_ = scale; }

        // Actions
        void DoMove(Vector2 to, float duration, Easing easing=Easing::kLinear, bool snapping=false, std::function<void()> onComplete=SPNULL);
        void DoMoveX(float to, float duration, Easing easing=Easing::kLinear, bool snapping=false, std::function<void()> onComplete=SPNULL);
        void DoMoveY(float to, float duration, Easing easing=Easing::kLinear, bool snapping=false, std::function<void()> onComplete=SPNULL);
        void DoLocalMove(Vector2 to, float duration, Easing easing=Easing::kLinear, bool snapping=false, std::function<void()> onComplete=SPNULL);
        void DoLocalMoveX(float to, float duration, Easing easing=Easing::kLinear, bool snapping=false, std::function<void()> onComplete=SPNULL);
        void DoLocalMoveY(float to, float duration, Easing easing=Easing::kLinear, bool snapping=false, std::function<void()> onComplete=SPNULL);

    protected:
        friend class GameObject;

        Transform();

        Graphics* graphics_;
        Vector2 position_, localPosition_;
        float rotation_, localRotation_;
        Vector2 localScale_;
};
}   // namespace snowpulse

#endif
