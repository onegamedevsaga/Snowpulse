#include "rect_renderer.h"

#include "../../common/singleton.h"
#include "../../application/application.h"

namespace snowpulse {
std::shared_ptr<RectRenderer> RectRenderer::Create(Vector2 size) {
    auto rectRenderer = std::shared_ptr<RectRenderer>(new RectRenderer("rect_renderer"));
    rectRenderer->LoadGraphics(Application::GetInstance()->GetGraphics());
    rectRenderer->SetSize(size);
    return rectRenderer;
}

RectRenderer::RectRenderer(std::string componentName) : Renderer(componentName) {
}

RectRenderer::~RectRenderer() {
}

void RectRenderer::LoadGraphics(Graphics* graphics) {
    Renderer::LoadGraphics(graphics);
}

// From Updatable
void RectRenderer::Update(float deltaTime) {
    Component::Update(deltaTime);
}

// From Drawable
void RectRenderer::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    graphics->DrawSprite(size_, "", worldMatrix, color_, sortOrder_, blendMode_);
}
}   // namespace snowpulse
