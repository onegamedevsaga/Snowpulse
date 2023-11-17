#include "spine_renderer.h"

#include "../../common/singleton.h"
#include "../../application/application.h"

namespace snowpulse {
std::shared_ptr<SpineRenderer> SpineRenderer::Create(std::string jsonFilename) {
    auto spineRenderer = std::shared_ptr<SpineRenderer>(new SpineRenderer());
    spineRenderer->filename_ = jsonFilename;
    spineRenderer->LoadGraphics(Application::GetInstance()->GetGraphics());
    return spineRenderer;
}

SpineRenderer::SpineRenderer() : SpriteRenderer("spine_renderer"), sortOrder_(0) {
}

SpineRenderer::~SpineRenderer() {
}

void SpineRenderer::LoadGraphics(Graphics* graphics) {
    graphics->LoadTexture(filename_, TextureFiltering::kBilinear);
    SetSize(graphics->GetTextureSize(filename_));
}

// From Updatable
void SpineRenderer::Update(float deltaTime) {
    Component::Update(deltaTime);
}

// From Drawable
void SpineRenderer::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    graphics->DrawSprite(size_, filename_, worldMatrix, color_, sortOrder_);
}
}   // namespace snowpulse
