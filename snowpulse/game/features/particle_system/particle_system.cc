#include "particle_system.h"

#include <iostream>
#include <map>

namespace snowpulse {
std::shared_ptr<ParticleSystem> ParticleSystem::Create(ParticleSystemSettings settings) {
    auto particleSystem = std::shared_ptr<ParticleSystem>(new ParticleSystem());
    particleSystem->settings_ = settings;
    return particleSystem;
}

ParticleSystem::ParticleSystem() {
    
}

ParticleSystem::~ParticleSystem() {
    
}

void ParticleSystem::Update(float deltaTime) {
    
}

void ParticleSystem::Draw(Graphics* graphics, Matrix4x4 worldMatrix, int sortOrder, bool isPremultiplied) {
}

void ParticleSystem::Play() {
    
}

}   // namespace snowpulse
