#include "plane.h"

namespace game {
std::shared_ptr<Plane> Plane::Create() {
    auto plane = std::shared_ptr<Plane>(new Plane());
    return plane;
}

Plane::Plane() : Component("plane") {
    isMouseDragging_ = false;
    resolutionSize_ = snowpulse::Application::GetInstance()->GetResolutionSize();
    auto screenSize = snowpulse::Application::GetInstance()->GetScreenSize();
    screenToResolutionFactor_ = snowpulse::Vector2((float)resolutionSize_.x / (float)screenSize.x, (float)resolutionSize_.y / (float)screenSize.y);

    auto actionPanelHeight = 40.0f;
    auto inspectorPanelWidth = 375.0f;
    input_ = snowpulse::Input::GetInstance();
    camera_ = snowpulse::Application::GetInstance()->GetGraphics()->GetCamera();
    defaultCameraPosition_ = snowpulse::Vector2(inspectorPanelWidth, actionPanelHeight) * screenToResolutionFactor_ * 0.5f;
    camera_->SetPosition(defaultCameraPosition_);
}

Plane::~Plane() {
}

void Plane::Update(float deltaTime) {
    if (input_->GetPressed("mouse_left")) {
        mouseDownPosition_ = input_->GetMousePositionOnScreen();
        auto camPos = camera_->GetPosition();
        cameraPositionOnMouseDown_.x = camPos.x;
        cameraPositionOnMouseDown_.y = camPos.y;
        isMouseDragging_ = true;
    }
    else if (isMouseDragging_ && input_->GetReleased("mouse_left")) {
        isMouseDragging_ = false;
    }
    else if (isMouseDragging_) {
        auto mousePos = input_->GetMousePositionOnScreen();
        auto mouseMovement = mousePos - mouseDownPosition_;
        auto newCamPos = cameraPositionOnMouseDown_ + (mouseMovement * screenToResolutionFactor_ * -1.0f);
        camera_->SetPosition(newCamPos);
    }
}

void Plane::Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) {
    auto cameraPos = graphics->GetCamera()->GetPosition();
    float lineWidth = 2.0f;
    snowpulse::Vector2 xLine(lineWidth, (float)resolutionSize_.y);
    snowpulse::Vector2 yLine((float)resolutionSize_.x, lineWidth);
    auto xLineMatrix = snowpulse::Matrix4x4(worldMatrix);
    auto yLineMatrix = snowpulse::Matrix4x4(worldMatrix);
    xLineMatrix.AddTranslate(snowpulse::Vector3(0.0f, cameraPos.y, 0.0f));
    yLineMatrix.AddTranslate(snowpulse::Vector3(cameraPos.x, 0.0f, 0.0f));
    graphics->DrawSprite(xLine, "", xLineMatrix, snowpulse::Color::White(), -9);
    graphics->DrawSprite(yLine, "", yLineMatrix, snowpulse::Color::White(), -9);
}
}   // namespace snowpulse
