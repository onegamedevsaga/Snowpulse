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
        p.position += p.velocity * deltaTime;
        p.velocity += settings_.acceleration * deltaTime;

        float percentage = 1.0f - p.currentLife / p.lifespan;
        p.color = settings_.colorStart + ((settings_.colorEnd - settings_.colorStart) * percentage);
        p.scale = settings_.scaleStartEnd.x + ((settings_.scaleStartEnd.y - settings_.scaleStartEnd.x) * percentage);

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
    if (settings_.emissionShape == ParticleSystemSettings::EmissionShape::kPoint) {
        data.position = Vector2();
    }
    else if (settings_.emissionShape == ParticleSystemSettings::EmissionShape::kRect) {
        float x = (float)(rand() % (int)(settings_.emissionRectSize.x * 100.0f)) * 0.01f;
        float y = (float)(rand() % (int)(settings_.emissionRectSize.y * 100.0f)) * 0.01f;
        x -= settings_.emissionRectSize.x * 0.5f;
        y -= settings_.emissionRectSize.y * 0.5f;
        data.position = Vector2(x, y);
    }
    else if (settings_.emissionShape == ParticleSystemSettings::EmissionShape::kCircle) {
        float magnitude = (float)(rand() % (int)(settings_.emissionRadius * 100.0f)) * 0.01f;
        float angle = (float)(rand() % 36000) * 0.01f;
        float x = cos(SPDEGTORAD(angle)) * magnitude;
        float y = sin(SPDEGTORAD(angle)) * magnitude;
        data.position = Vector2(x, y);
    }

    if (settings_.lifespanValueMode == ParticleSystemSettings::ValueMode::kSingle) {
        data.lifespan = settings_.lifespanA;
    }
    else if (settings_.lifespanValueMode == ParticleSystemSettings::ValueMode::kRandomBetween) {
        data.lifespan = rand() % 2 == 0 ? settings_.lifespanA : settings_.lifespanB;
    }
    else if (settings_.lifespanValueMode == ParticleSystemSettings::ValueMode::kRandomWithin) {
        float a = settings_.lifespanA < settings_.lifespanB ? settings_.lifespanA : settings_.lifespanB;
        float b = settings_.lifespanA < settings_.lifespanB ? settings_.lifespanB : settings_.lifespanA;
        float range = abs(b - a);
        data.lifespan = a + (float)(rand() % (int)(range * 100.0f)) * 0.01f;
    }

    float speed = 0.0f;
    Vector2 direction;

    if (settings_.speedValueMode == ParticleSystemSettings::ValueMode::kSingle) {
        speed = settings_.speedA;
    }
    else if (settings_.speedValueMode == ParticleSystemSettings::ValueMode::kRandomBetween) {
        speed = rand() % 2 == 0 ? settings_.speedA : settings_.speedB;
    }
    else if (settings_.speedValueMode == ParticleSystemSettings::ValueMode::kRandomWithin) {
        float a = settings_.speedA < settings_.speedB ? settings_.speedA : settings_.speedB;
        float b = settings_.speedA < settings_.speedB ? settings_.speedB : settings_.speedA;
        float range = abs(b - a);
        speed = a + (float)(rand() % (int)(range * 100.0f)) * 0.01f;
    }

    if (settings_.emissionAngleValueMode == ParticleSystemSettings::ValueMode::kSingle) {
        direction.x = cos(SPDEGTORAD(settings_.emissionAngleA));
        direction.y = sin(SPDEGTORAD(settings_.emissionAngleA));
    }
    else if (settings_.emissionAngleValueMode == ParticleSystemSettings::ValueMode::kRandomBetween) {
        auto angle = rand() % 2 == 0 ? settings_.emissionAngleA : settings_.emissionAngleB;
        direction.x = cos(SPDEGTORAD(angle));
        direction.y = sin(SPDEGTORAD(angle));
    }
    else if (settings_.emissionAngleValueMode == ParticleSystemSettings::ValueMode::kRandomWithin) {
        float a = settings_.emissionAngleA < settings_.emissionAngleB ? settings_.emissionAngleA : settings_.emissionAngleB;
        float b = settings_.emissionAngleA < settings_.emissionAngleB ? settings_.emissionAngleB : settings_.emissionAngleA;
        float range = abs(b - a);
        auto angle = a + (float)(rand() % (int)(range * 100.0f)) * 0.01f;
        direction.x = cos(SPDEGTORAD(angle));
        direction.y = sin(SPDEGTORAD(angle));
    }

    data.currentLife = data.lifespan;
    data.velocity = direction * speed;
    data.rotation = settings_.angleStart;
    data.scale = settings_.scaleStartEnd.x;
    data.size = Application::GetInstance()->GetGraphics()->GetTextureSize(settings_.textureFilename);
    particles_.push_back(data);
    return true;
}

}   // namespace snowpulse
