#include "camera.h"

#include <iostream>

namespace snowpulse {

std::shared_ptr<Camera> Camera::Create(const Vector2Int& resolution) {
    auto camera = new Camera();
    auto res = Vector2(resolution.x, resolution.y);
    camera->SetPosition(res * -0.5f);
    return std::shared_ptr<Camera>(camera);
}

Camera::Camera() : position_(0.0f, 0.0f, -10.0f) {
}

Camera::~Camera() {
}

Matrix4x4 Camera::GetMatrix() const {
    return Matrix4x4::LookAtRH(position_, Vector3(position_.x, position_.y, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
}

void Camera::SetPosition(Vector2 position) {
    position_.x = position.x;
    position_.y = position.y;

#ifdef SPDEBUG
        std::cout << "Camera Position at (" << position_.x << ", " << position_.y << ")" << std::endl;
#endif
}
}   // namespace snowpulse
