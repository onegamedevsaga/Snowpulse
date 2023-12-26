#include "camera.h"

#include <iostream>

#include "../application/application.h"

namespace snowpulse {

std::shared_ptr<Camera> Camera::Create() {
    auto camera = new Camera();
    auto app = Application::GetInstance();
    auto screenSize = app->GetScreenSize();
    auto resolutionSize = app->GetResolutionSize();
    camera->application_ = app;
    camera->screenSize_ = Vector2(screenSize.x, screenSize.y);
    camera->resolutionSize_ = Vector2(resolutionSize.x, resolutionSize.y);
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

void Camera::SetRawPosition(Vector2 position) {
    auto res = application_->GetResolutionSize();
    auto offset = Vector3((float)res.x * -0.5f, (float)res.y * -0.5f);
    position_.x = position.x - offset.x;
    position_.y = position.y - offset.y;

#ifdef SPDEBUG
        std::cout << "Camera Position at (" << position_.x << ", " << position_.y << ")" << std::endl;
#endif
}

Vector2 Camera::ConvertScreenToWorld(Vector2 screenPos) {
    auto factor = resolutionSize_ / screenSize_;
    auto converted = screenPos * factor;
    auto camRawPos = GetRawPosition();
    converted.x += camRawPos.x;
    converted.y += camRawPos.y;
    return converted;
}

Vector2 Camera::ConvertWorldToScreen(Vector2 worldPos) {
    auto factor = screenSize_ / resolutionSize_;
    auto camRawPos = GetRawPosition();
    worldPos.x -= camRawPos.x;
    worldPos.y -= camRawPos.y;
    auto converted = worldPos * factor;
    return converted;
}
}   // namespace snowpulse
