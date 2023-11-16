#include "sprite_renderer.h"

#include "../../common/singleton.h"
#include "../../application/application.h"

namespace snowpulse {
std::shared_ptr<SpriteRenderer> SpriteRenderer::Create(std::string filename) {
    auto spriteRenderer = std::shared_ptr<SpriteRenderer>(new SpriteRenderer());
    spriteRenderer->filename_ = filename;
    auto x = Application::GetInstance();
    spriteRenderer->LoadGraphics(x->GetGraphics());
    return spriteRenderer;
}

SpriteRenderer::SpriteRenderer() : Component("sprite_renderer"), sortOrder_(0) {
}

SpriteRenderer::~SpriteRenderer() {
}

void SpriteRenderer::LoadGraphics(Graphics* graphics) {
    graphics->LoadTexture(filename_, TextureFiltering::kBilinear);
    SetSize(graphics->GetTextureSize(filename_));
}

// From Updatable
void SpriteRenderer::Update(float deltaTime) {
    Component::Update(deltaTime);
}

// From Drawable
void SpriteRenderer::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    graphics->DrawSprite(size_, filename_, worldMatrix, color_, sortOrder_);
}
}   // namespace snowpulse
