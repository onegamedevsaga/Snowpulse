#include "plane_renderer.h"

namespace game {
static const float kActionPanelHeight = 40.0f;
static const float kInspectorPanelWidth = 375.0f;

std::shared_ptr<PlaneRenderer> PlaneRenderer::Create() {
    auto plane = std::shared_ptr<PlaneRenderer>(new PlaneRenderer());
    return plane;
}

PlaneRenderer::PlaneRenderer() : Renderer("plane_renderer") {
    isMouseDragging_ = false;
    auto screenToResolutionFactor = GetScreenToResolutionFactor();

    input_ = snowpulse::Input::GetInstance();
    camera_ = snowpulse::Application::GetInstance()->GetGraphics()->GetCamera();
    auto defaultCameraPosition = snowpulse::Vector2(kInspectorPanelWidth, kActionPanelHeight) * screenToResolutionFactor * 0.5f;
    camera_->SetPosition(defaultCameraPosition);
}

PlaneRenderer::~PlaneRenderer() {
}

void PlaneRenderer::Update(float deltaTime) {
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
        auto cameraSize = camera_->GetSize();
        auto screenToResolutionFactor = GetScreenToResolutionFactor();
        auto mousePos = input_->GetMousePositionOnScreen();
        auto mouseMovement = mousePos - mouseDownPosition_;
        auto newCamPos = cameraPositionOnMouseDown_ + (mouseMovement * screenToResolutionFactor * cameraSize * -1.0f);
        camera_->SetPosition(newCamPos);
    }

    if (input_->GetPressed("right_bracket")) {
        isMouseDragging_ = false;
        auto cameraSize = camera_->GetSize();
        auto newCameraSize = cameraSize * 0.8f;
        auto sizeDelta = cameraSize - newCameraSize;
        auto cameraPos = camera_->GetPosition();
        camera_->SetSize(newCameraSize);
        camera_->SetPosition(snowpulse::Vector2(cameraPos.x - kInspectorPanelWidth * sizeDelta, cameraPos.y - kActionPanelHeight * sizeDelta));
    }
    else if (input_->GetPressed("left_bracket")) {
        isMouseDragging_ = false;
        auto cameraSize = camera_->GetSize();
        auto newCameraSize = cameraSize * 1.2f;
        auto sizeDelta = cameraSize - newCameraSize;
        auto cameraPos = camera_->GetPosition();
        camera_->SetSize(newCameraSize);
        camera_->SetPosition(snowpulse::Vector2(cameraPos.x - kInspectorPanelWidth * sizeDelta, cameraPos.y - kActionPanelHeight * sizeDelta));
    }

    if (input_->GetPressed("r")) {
        isMouseDragging_ = false;
        auto screenToResolutionFactor = GetScreenToResolutionFactor();
        auto defaultCameraPosition = snowpulse::Vector2(kInspectorPanelWidth, kActionPanelHeight) * screenToResolutionFactor * 0.5f;
        camera_->SetSize(1.0f);
        camera_->SetPosition(defaultCameraPosition);
    }
}

void PlaneRenderer::Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) {
    auto resolutionSize = snowpulse::Application::GetInstance()->GetResolutionSize();
    auto cameraPos = camera_->GetPosition();
    auto cameraSize = camera_->GetSize();
    float lineWidth = 2.0f;
    snowpulse::Vector2 xLine(lineWidth * cameraSize, (float)resolutionSize.y * cameraSize);
    snowpulse::Vector2 yLine((float)resolutionSize.x * cameraSize, lineWidth * cameraSize);
    auto xLineMatrix = snowpulse::Matrix4x4(worldMatrix);
    auto yLineMatrix = snowpulse::Matrix4x4(worldMatrix);
    xLineMatrix.AddTranslate(snowpulse::Vector3(0.0f, cameraPos.y, 0.0f));
    yLineMatrix.AddTranslate(snowpulse::Vector3(cameraPos.x, 0.0f, 0.0f));

    int gridSpacing = 100;
    int xCount = (resolutionSize.x * cameraSize) / gridSpacing;
    int yCount = (resolutionSize.y * cameraSize) / gridSpacing;
    auto xGridLineMatrix = snowpulse::Matrix4x4(xLineMatrix);
    auto yGridLineMatrix = snowpulse::Matrix4x4(yLineMatrix);
    xGridLineMatrix.AddTranslate(snowpulse::Vector3(gridSpacing * -xCount, 0.0f, 0.0f));
    yGridLineMatrix.AddTranslate(snowpulse::Vector3(0.0f, gridSpacing * -yCount, 0.0f));
    auto gridColor = color_;
    gridColor.a = 0.2f;
    for (int x = 0; x < xCount * 2; x++) {
        xGridLineMatrix.AddTranslate(snowpulse::Vector3(gridSpacing, 0.0f, 0.0f));
        graphics->DrawSprite(xLine, "", xGridLineMatrix, gridColor, sortOrder_, blendMode_, snowpulse::TextureFiltering::kBilinear, isPremultiplied_);
    }
    for (int y = 0; y < yCount * 2; y++) {
        yGridLineMatrix.AddTranslate(snowpulse::Vector3(0.0f, gridSpacing, 0.0f));
        graphics->DrawSprite(yLine, "", yGridLineMatrix, gridColor, sortOrder_, blendMode_, snowpulse::TextureFiltering::kBilinear, isPremultiplied_);
    }

    graphics->DrawSprite(xLine, "", xLineMatrix, snowpulse::Color::Green(), sortOrder_, blendMode_, snowpulse::TextureFiltering::kBilinear, isPremultiplied_);
    graphics->DrawSprite(yLine, "", yLineMatrix, snowpulse::Color::Red(), sortOrder_, blendMode_, snowpulse::TextureFiltering::kBilinear, isPremultiplied_);
}

snowpulse::Vector2 PlaneRenderer::GetScreenToResolutionFactor() {
    auto resolutionSize = snowpulse::Application::GetInstance()->GetResolutionSize();
    auto screenSize = snowpulse::Application::GetInstance()->GetScreenSize();
    return snowpulse::Vector2((float)resolutionSize.x / (float)screenSize.x, (float)resolutionSize.y / (float)screenSize.y);
}
}   // namespace snowpulse
