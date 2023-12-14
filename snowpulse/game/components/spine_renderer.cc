#include "spine_renderer.h"

#include "../../application/application.h"

namespace snowpulse {
std::shared_ptr<SpineRenderer> SpineRenderer::Create(std::string jsonFilename, std::string atlasFilename, TextureFiltering filtering) {
    auto spineRenderer = std::shared_ptr<SpineRenderer>(new SpineRenderer());
    spineRenderer->skeletonAnimation_ = SpineSkeletonAnimation::Create(jsonFilename, atlasFilename, filtering);
    spineRenderer->filename_ = jsonFilename;
    spineRenderer->textureFiltering_ = filtering;
    spineRenderer->LoadGraphics(Application::GetInstance()->GetGraphics());
    return spineRenderer;
}

SpineRenderer::SpineRenderer() : SpriteRenderer("spine_renderer") {
}

SpineRenderer::~SpineRenderer() {
}

void SpineRenderer::LoadGraphics(Graphics* graphics) {
}

// From Updatable
void SpineRenderer::Update(float deltaTime) {
    Component::Update(deltaTime);
    if (skeletonAnimation_) {
        skeletonAnimation_->Update(deltaTime);
    }
}

// From Drawable
void SpineRenderer::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    if (skeletonAnimation_) {
        skeletonAnimation_->Draw(graphics, worldMatrix, sortOrder_, isPremultiplied_);
    }
}

void SpineRenderer::SetSkin(std::string name) {
    if (skeletonAnimation_) {
        skeletonAnimation_->SetSkin(name);
    }
}

void SpineRenderer::PlayAnimation(std::string name, bool looping) {
    if (skeletonAnimation_) {
        skeletonAnimation_->PlayAnimation(name, looping);
    }
}
}   // namespace snowpulse
