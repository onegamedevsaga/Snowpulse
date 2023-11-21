#include "sprite_renderer.h"

#include "../../common/singleton.h"
#include "../../application/application.h"

namespace snowpulse {
std::shared_ptr<SpriteRenderer> SpriteRenderer::Create(std::string filename, TextureFiltering filtering) {
    auto spriteRenderer = std::shared_ptr<SpriteRenderer>(new SpriteRenderer());
    spriteRenderer->filename_ = filename;
    spriteRenderer->textureFiltering_ = filtering;
    spriteRenderer->LoadGraphics(Application::GetInstance()->GetGraphics());
    return spriteRenderer;
}

SpriteRenderer::SpriteRenderer() : Component("sprite_renderer"), sortOrder_(0), blendMode_(BlendMode::kNormal), isPremultiplied_(false) {
}

SpriteRenderer::SpriteRenderer(std::string componentName) : Component(componentName), sortOrder_(0)  {
}

SpriteRenderer::~SpriteRenderer() {
}

void SpriteRenderer::LoadGraphics(Graphics* graphics) {
    graphics->LoadTexture(filename_, textureFiltering_);
    SetSize(graphics->GetTextureSize(filename_, textureFiltering_));
}

// From Updatable
void SpriteRenderer::Update(float deltaTime) {
    Component::Update(deltaTime);
}

// From Drawable
void SpriteRenderer::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    graphics->DrawSprite(size_, graphics->GetTextureFullFilename(filename_, textureFiltering_), worldMatrix, color_, sortOrder_, blendMode_, isPremultiplied_);
}
}   // namespace snowpulse
