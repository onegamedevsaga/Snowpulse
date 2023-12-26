#include "particle_effects_renderer.h"

#include "../../application/application.h"

namespace snowpulse {
std::shared_ptr<ParticleEffectsRenderer> ParticleEffectsRenderer::Create(const ParticleSystemSettings& settings) {
    auto particleEffectsRenderer = std::shared_ptr<ParticleEffectsRenderer>(new ParticleEffectsRenderer());
    particleEffectsRenderer->LoadGraphics(Application::GetInstance()->GetGraphics(), settings);
    return particleEffectsRenderer;
}

ParticleEffectsRenderer::ParticleEffectsRenderer() : Renderer("particle_effects_renderer") {
}

ParticleEffectsRenderer::~ParticleEffectsRenderer() {
}

void ParticleEffectsRenderer::LoadGraphics(Graphics* graphics, const ParticleSystemSettings& settings) {
    Renderer::LoadGraphics(graphics);
    particleSystem_ = ParticleSystem::Create(settings);
    SetSettings(settings);
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

void ParticleEffectsRenderer::SetSettings(const ParticleSystemSettings& settings) {
    particleSystem_->SetSettings(settings);
}

void ParticleEffectsRenderer::Play() {
    particleSystem_->Play();
}

int ParticleEffectsRenderer::GetDrawnParticleCount() const {
    return particleSystem_->GetDrawnParticleCount();
}
}   // namespace snowpulse
