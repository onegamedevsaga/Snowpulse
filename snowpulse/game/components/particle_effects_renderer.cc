#include "particle_effects_renderer.h"

#include "../../application/application.h"
#include "../../application/directory.h"

namespace snowpulse {
std::shared_ptr<ParticleEffectsRenderer> ParticleEffectsRenderer::Create(std::string filename, PathType pathType) {
    auto particleEffectsRenderer = std::shared_ptr<ParticleEffectsRenderer>(new ParticleEffectsRenderer());
    ParticleSystemSettings settings;
    if (settings.LoadFromSPPEFile(filename, pathType)) {
        auto path = Directory::GetInstance()->GetPathFromFilename(filename);
        auto spriteFilename = Directory::GetInstance()->GetFilenameFromPath(settings.textureFilename);
        settings.textureFilename = path + spriteFilename;
        particleEffectsRenderer->LoadGraphics(Application::GetInstance()->GetGraphics(), settings);
    }
    return particleEffectsRenderer;
}

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
    if (particleSystem_) {
        particleSystem_->Update(deltaTime);
    }
}

// From Drawable
void ParticleEffectsRenderer::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    particleSystem_->Draw(graphics, worldMatrix, sortOrder_, isPremultiplied_);
}

void ParticleEffectsRenderer::SetSettings(const ParticleSystemSettings& settings) {
    if (particleSystem_) {
        particleSystem_->SetSettings(settings);
    }
}

void ParticleEffectsRenderer::Play() {
    if (particleSystem_) {
        particleSystem_->Play();
    }
}

int ParticleEffectsRenderer::GetDrawnParticleCount() const {
    if (particleSystem_) {
        return particleSystem_->GetDrawnParticleCount();
    }
    return -1;
}
}   // namespace snowpulse
