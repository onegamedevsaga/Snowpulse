#include "particle_system.h"

#include <iostream>
#include <map>

#include "../../../application/application.h"

namespace snowpulse {
std::shared_ptr<ParticleSystem> ParticleSystem::Create(const ParticleSystemSettings& settings) {
    auto particleSystem = std::shared_ptr<ParticleSystem>(new ParticleSystem());
    particleSystem->settings_ = ParticleSystemSettings(settings);
    return particleSystem;
}

ParticleSystem::ParticleSystem() : isPlaying_(false) {
    particles_.reserve(settings_.maxParticleCount);
}

ParticleSystem::~ParticleSystem() {
    
}

void ParticleSystem::Update(float deltaTime) {
    emissionTime_ -= deltaTime;
    if (emissionTime_ <= 0.0f) {
        if (SpawnParticle()) {
            emissionTime_ = 1.0f / settings_.emissionRate;
        }
    }

    std::vector<ParticleData> stillAliveParticles;
    stillAliveParticles.reserve(particles_.size());
    for (auto& p : particles_) {
        p.currentLife -= deltaTime;
        if (p.currentLife <= 0.0f) {
            continue;
        }
        p.position += p.direction * p.speed * deltaTime;
        stillAliveParticles.push_back(p);
    }
    particles_.swap(stillAliveParticles);
}

void ParticleSystem::Draw(Graphics* graphics, Matrix4x4 worldMatrix, int sortOrder, bool isPremultiplied) {
    for (auto& p : particles_) {
        Matrix4x4 particleMatrix =  Matrix4x4::Scale(Vector3(p.scale, p.scale, p.scale)) *
                                    Matrix4x4::Rotate(Vector3(0.0f, 0.0f, p.rotation)) *
                                    Matrix4x4::Translate(Vector3(p.position.x, p.position.y, 0.0f)) *
                                    worldMatrix;
        graphics->DrawSprite(p.size, settings_.textureFilename, particleMatrix, p.color, sortOrder, settings_.blendMode, settings_.textureFiltering, isPremultiplied);
    }
}

void ParticleSystem::Play() {
    
}

bool ParticleSystem::SpawnParticle() {
    if ((int)particles_.size() >= settings_.maxParticleCount) {
        return false;
    }

    ParticleData data;
    data.lifespan = settings_.lifespanA;
    data.currentLife = data.lifespan;
    float angle = (float)(rand() % 3600) * 0.1f;
    angle = angle * (3.1416f / 180.0f);
    Vector2 dir(cos(angle), sin(angle));
    data.direction = dir;
    data.speed = settings_.speedA;
    data.rotation = settings_.startAngle;
    data.scale = settings_.startScale;
    data.size = Application::GetInstance()->GetGraphics()->GetTextureSize(settings_.textureFilename);
    particles_.push_back(data);
    return true;
}

}   // namespace snowpulse
