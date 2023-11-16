#include "transform.h"

#include "../../application/application.h"
#include "../game_object.h"

#include "../actions/action_move.h"

namespace snowpulse {
std::shared_ptr<Transform> Transform::Create() {
    auto transform = std::shared_ptr<Transform>(new Transform());
    return transform;
}

Transform::Transform() :    Component("transform"),
                            localPosition_(0.0f, 0.0f),
                            localRotation_(0.0f),
                            localScale_(1.0f, 1.0f) {
    graphics_ = Application::GetInstance()->GetGraphics();
}

Transform::~Transform() {
}

// From Updatable
void Transform::Update(float deltaTime) {

}

Matrix4x4 Transform::GetLocalMatrix(GameObject* gameObject) {
    if (!gameObject) {
        return  Matrix4x4::Scale(GetLocalScaleVector3()) *
                Matrix4x4::Rotate(GetLocalRotationVector3()) *
                Matrix4x4::Translate(GetLocalPositionVector3());
    }
    return  Matrix4x4::Scale(gameObject->GetTransform()->GetLocalScaleVector3()) *
            Matrix4x4::Rotate(gameObject->GetTransform()->GetLocalRotationVector3()) *
            Matrix4x4::Translate(gameObject->GetTransform()->GetLocalPositionVector3());
}

Matrix4x4 Transform::GetParentWorldMatrix() {
    auto parentNode = gameObject_->GetParent();
    if (parentNode) {
        auto parentGO = dynamic_cast<GameObject*>(parentNode);
        if (parentGO) {
            auto localMatrix = GetLocalMatrix(parentGO);
            return localMatrix * parentGO->GetTransform()->GetParentWorldMatrix();
        }
    }
    return Matrix4x4();
}

float Transform::GetParentWorldRotation() {
    auto parentNode = gameObject_->GetParent();
    if (parentNode) {
        auto parentGO = dynamic_cast<GameObject*>(parentNode);
        if (parentGO) {
            auto localRotation = parentGO->GetTransform()->GetLocalRotation();
            return localRotation + parentGO->GetTransform()->GetParentWorldRotation();
        }
    }
    return 0.0f;
}

void Transform::SetPosition(Vector2 position) {
    position_ = position;
    auto invWorldMatrix = graphics_->InvertMatrixNatively(GetParentWorldMatrix());
    auto local = invWorldMatrix * Vector3(position_.x, position_.y, 0.0f);
    localPosition_.x = local.x;
    localPosition_.y = local.y;
}

void Transform::SetLocalPosition(Vector2 position) {
    localPosition_ = position;
    auto world = GetParentWorldMatrix() * Vector3(localPosition_.x, localPosition_.y, 0.0f);
    position_.x = world.x;
    position_.y = world.y;
}

void Transform::SetRotation(float rotation) {
    rotation_ = rotation;
    localRotation_ = rotation_ - GetParentWorldRotation();
}

void Transform::SetLocalRotation(float rotation) {
    localRotation_ = rotation;
    rotation_ = localRotation_ + GetParentWorldRotation();
}

void Transform::DoMove(Vector2 to, float duration, Easing easing, bool snapping, std::function<void()> onComplete) {
    ActionMove::Create( to,
                        GetPosition(),
                        this,
                        duration,
                        Space::kWorld,
                        easing)->OnComplete([onComplete](Action* a) {
        if (onComplete) {
            onComplete();
        }
    })->Run();
}
void Transform::DoMoveX(float to, float duration, Easing easing, bool snapping, std::function<void()> onComplete) {
    ActionMove::Create( Vector2(to, position_.y),
                        GetPosition(),
                        this,
                        duration,
                        Space::kWorld,
                        easing)->OnComplete([onComplete](Action* a) {
        if (onComplete) {
            onComplete();
        }
    })->Run();
}
void Transform::DoMoveY(float to, float duration, Easing easing, bool snapping, std::function<void()> onComplete) {
    ActionMove::Create( Vector2(position_.x, to),
                        GetPosition(),
                        this,
                        duration,
                        Space::kWorld,
                        easing)->OnComplete([onComplete](Action* a) {
        if (onComplete) {
            onComplete();
        }
    })->Run();
}
void Transform::DoLocalMove(Vector2 to, float duration, Easing easing, bool snapping, std::function<void()> onComplete) {
    ActionMove::Create( to,
                        GetLocalPosition(),
                        this,
                        duration,
                        Space::kLocal,
                        easing)->OnComplete([onComplete](Action* a) {
        if (onComplete) {
            onComplete();
        }
    })->Run();
}
void Transform::DoLocalMoveX(float to, float duration, Easing easing, bool snapping, std::function<void()> onComplete) {
    ActionMove::Create( Vector2(to, localPosition_.y),
                        GetLocalPosition(),
                        this,
                        duration,
                        Space::kLocal,
                        easing)->OnComplete([onComplete](Action* a) {
        if (onComplete) {
            onComplete();
        }
    })->Run();
}
void Transform::DoLocalMoveY(float to, float duration, Easing easing, bool snapping, std::function<void()> onComplete) {
    ActionMove::Create( Vector2(localPosition_.x, to),
                        GetLocalPosition(),
                        this,
                        duration,
                        Space::kLocal,
                        easing)->OnComplete([onComplete](Action* a) {
        if (onComplete) {
            onComplete();
        }
    })->Run();
}
}   // namespace snowpulse
