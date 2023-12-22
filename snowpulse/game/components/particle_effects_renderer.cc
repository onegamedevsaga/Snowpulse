#include "particle_effects_renderer.h"

#include "../../application/application.h"

namespace snowpulse {
std::shared_ptr<ParticleEffectsRenderer> ParticleEffectsRenderer::Create(const ParticleSystemSettings& settings) {
    auto particleEffectsRenderer = std::shared_ptr<ParticleEffectsRenderer>(new ParticleEffectsRenderer());
    particleEffectsRenderer->LoadGraphics(Application::GetInstance()->GetGraphics(), settings);
    return particleEffectsRenderer;
}

ParticleEffectsRenderer::ParticleEffectsRenderer() : SpriteRenderer("particle_effects_renderer") {
}

ParticleEffectsRenderer::~ParticleEffectsRenderer() {
}

void ParticleEffectsRenderer::LoadGraphics(Graphics* graphics, const ParticleSystemSettings& settings) {
    graphics_ = graphics;
    graphics_->LoadTexture(settings.textureFilename);
    particleSystem_ = ParticleSystem::Create(settings);
}

// From Updatable
void ParticleEffectsRenderer::Update(float deltaTime) {
    Component::Update(deltaTime);
    particleSystem_->Update(deltaTime);
}

// From Drawable
void ParticleEffectsRenderer::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    particleSystem_->Draw(graphics, worldMatrix, sortOrder_, isPremultiplied_);
}
}   // namespace snowpulse
