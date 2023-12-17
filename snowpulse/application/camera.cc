#include "camera.h"

#include <iostream>

#include "../application/application.h"

namespace snowpulse {

std::shared_ptr<Camera> Camera::Create() {
    auto camera = new Camera();
    camera->application_ = Application::GetInstance();
    camera->SetPosition(Vector2(0.0f, 0.0f));
    return std::shared_ptr<Camera>(camera);
}

Camera::Camera() : position_(0.0f, 0.0f, -10.0f) {
}

Camera::~Camera() {
}

Matrix4x4 Camera::GetMatrix() const {
    auto pos = GetRawPosition();
    return Matrix4x4::LookAtRH(pos, Vector3(pos.x, pos.y, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
}

Vector3 Camera::GetRawPosition() const {
    auto res = application_->GetResolutionSize();
    auto offset = Vector3((float)res.x * -0.5f, (float)res.y * -0.5f);
    return position_ + offset;
}

void Camera::SetPosition(Vector2 position) {
    position_.x = position.x;
    position_.y = position.y;

#ifdef SPDEBUG
        std::cout << "Camera Position at (" << position_.x << ", " << position_.y << ")" << std::endl;
#endif
}
}   // namespace snowpulse
