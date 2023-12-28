#include "scene_main.h"

#include <iostream>
#include <functional>

#include "components/plane_renderer.h"
#include "components/action_panel.h"
#include "components/inspector_panel.h"

namespace game {
std::shared_ptr<SceneMain> SceneMain::Create(snowpulse::Application* application) {
    auto scene = std::shared_ptr<SceneMain>(new SceneMain());
    scene->Initialize(application);
    return scene;
}

SceneMain::SceneMain() : snowpulse::Scene() {
}

SceneMain::~SceneMain() {
#ifdef SPDEBUG
    std::cout << "SceneMain destroyed." << std::endl;
#endif
}

void SceneMain::Initialize(snowpulse::Application* application) {
    Scene::Initialize(application);
#ifdef SPDEBUG
    std::cout << "SceneLevel initialized." << std::endl;
#endif
}

void SceneMain::Shutdown() {
    
#ifdef SPDEBUG
    std::cout << "SceneMain shutdown." << std::endl;
#endif
    snowpulse::Scene::Shutdown();
}

void SceneMain::Start() {
    timeLapsed_ = 0.0f;

    auto resolutionSize = GetApplication()->GetResolutionSize();
    auto backgroundGo = snowpulse::GameObject::Create("backgroundGo");
    background_ = snowpulse::RectRenderer::Create(snowpulse::Vector2(resolutionSize.x, resolutionSize.y));
    background_->SetColor(snowpulse::Color(0.3f, 0.3f, 0.3f));
    background_->SetSortOrder(-10);
    backgroundGo->AddComponent(background_);
    AddChild(backgroundGo);

    auto planeGo = snowpulse::GameObject::Create("planeGo");
    auto planeRenderer = PlaneRenderer::Create();
    planeRenderer->SetSortOrder(-9);
    planeGo->AddComponent(planeRenderer);
    AddChild(planeGo);

    auto statsGo = snowpulse::GameObject::Create("statsGo");
    statsRenderer_ = snowpulse::FontRenderer::Create("fonts/roboto/Roboto-Bold.ttf", 30.0f, snowpulse::PathType::kDefaults, snowpulse::TextureFiltering::kAnisotropic);
    statsRenderer_->SetColor(snowpulse::Color(0.1f, 0.1f, 0.1f, 0.7f));
    statsRenderer_->SetSortOrder(10);
    statsGo->AddComponent(statsRenderer_);
    AddChild(statsGo);

    auto effectsGo = snowpulse::GameObject::Create("effectsGo");
    effectRenderer_ = snowpulse::ParticleEffectsRenderer::Create(snowpulse::ParticleSystemSettings());
    effectRenderer_->Play();
    effectsGo->AddComponent(effectRenderer_);
    AddChild(effectsGo);

    auto actionGo = snowpulse::GameObject::Create("actionGo");
    actionPanel_ = ActionPanel::Create();
    actionPanel_->SetOnLoadListener(std::bind(&SceneMain::OnActionLoadFile, this, std::placeholders::_1));
    actionGo->AddComponent(actionPanel_);
    AddChild(actionGo);

    auto inspectorGo = snowpulse::GameObject::Create("inspectorGo");
    inspectorPanel_ = InspectorPanel::Create();
    inspectorPanel_->SetListener(std::bind(&SceneMain::OnInspectorInvalidate, this, std::placeholders::_1));
    inspectorPanel_->Invalidate();
    inspectorGo->AddComponent(inspectorPanel_);
    AddChild(inspectorGo);

    actionPanel_->CreateNewEffect();
}

void SceneMain::Update(float deltaTime) {
    Scene::Update(deltaTime);
    auto resolutionSize = GetApplication()->GetResolutionSize();
    auto camera = GetCamera();
    auto cameraPos = camera->GetPosition();
    auto cameraSize = camera->GetSize();
    background_->SetSize(snowpulse::Vector2(resolutionSize.x * cameraSize, resolutionSize.y * cameraSize));
    background_->GetTransform()->SetPosition(snowpulse::Vector2(cameraPos.x, cameraPos.y));

    statsRenderer_->GetTransform()->SetPosition(snowpulse::Vector2(resolutionSize.x * -0.5f * cameraSize + cameraPos.x + (20.0f * cameraSize), resolutionSize.y * 0.5f * cameraSize + cameraPos.y - (160.0f * cameraSize)));
    statsRenderer_->GetTransform()->SetLocalScale(cameraSize);
    timeLapsed_ += deltaTime;
    if (timeLapsed_ >= 1.0f) {
        int fps = (int)(1.0f / deltaTime + 0.5f);
        int particleCount = effectRenderer_->GetDrawnParticleCount();
        statsRenderer_->SetText("FPS: " + std::to_string(fps) + "\nParticles Rendering: " + std::to_string(particleCount));
        timeLapsed_ -= 1.0f;
    }

#ifdef SPDEBUG
    std::cout << "SceneMain updating.. (deltaTime: " << deltaTime << ")" << std::endl;
#endif
}

void SceneMain::OnInspectorInvalidate(snowpulse::ParticleSystemSettings settings) {
    actionPanel_->CopySettings(settings);
    settings.texturePathType = snowpulse::PathType::kRaw;
    effectRenderer_->SetSettings(settings);
}

void SceneMain::OnActionLoadFile(snowpulse::ParticleSystemSettings& settings) {
    inspectorPanel_->SetSettings(settings);
    settings.texturePathType = snowpulse::PathType::kRaw;
    effectRenderer_->SetSettings(settings);
}
}
