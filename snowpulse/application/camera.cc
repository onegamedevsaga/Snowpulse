#include "camera.h"

#include <iostream>

namespace snowpulse {

std::shared_ptr<Camera> Camera::Create(const Vector2Int& resolution) {
    auto camera = new Camera();
    auto res = Vector3(resolution.x, resolution.y, 0.0f);
    camera->offset_ = res * -0.5f;
    camera->SetPosition(Vector2(0.0f, 0.0f));
    return std::shared_ptr<Camera>(camera);
}

Camera::Camera() : position_(0.0f, 0.0f, -10.0f) {
}

Camera::~Camera() {
}

Matrix4x4 Camera::GetMatrix() const {
    auto pos = position_ + offset_;
    return Matrix4x4::LookAtRH(pos, Vector3(pos.x, pos.y, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
}

void Camera::SetPosition(Vector2 position) {
    position_.x = position.x;
    position_.y = position.y;

#ifdef SPDEBUG
        std::cout << "Camera Position at (" << position_.x << ", " << position_.y << ")" << std::endl;
#endif
}
}   // namespace snowpulse
