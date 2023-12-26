#include "scene_main.h"

#include <iostream>

#include "components/plane.h"
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
    resolutionSize_ = GetApplication()->GetResolutionSize();

    auto backgroundGo = snowpulse::GameObject::Create("backgroundGo");
    background_ = snowpulse::SpriteRenderer::Create("");
    background_->SetSize(snowpulse::Vector2(resolutionSize_.x, resolutionSize_.y));
    background_->SetColor(snowpulse::Color(0.3f, 0.3f, 0.3f));
    background_->SetSortOrder(-10);
    backgroundGo->AddComponent(background_);
    AddChild(backgroundGo);

    auto planeGo = snowpulse::GameObject::Create("planeGo");
    auto plane = Plane::Create();
    //plane->SetSortOrder(-9);
    planeGo->AddComponent(plane);
    AddChild(planeGo);

    auto actionGo = snowpulse::GameObject::Create("actionGo");
    auto actionPanel = ActionPanel::Create();
    actionGo->AddComponent(actionPanel);
    AddChild(actionGo);

    auto inspectorGo = snowpulse::GameObject::Create("inspectorGo");
    auto inspectorPanel = InspectorPanel::Create();
    inspectorGo->AddComponent(inspectorPanel);
    AddChild(inspectorGo);

    snowpulse::ParticleSystemSettings effectsSettings;
    effectsSettings.texturePathType = snowpulse::PathType::kDefaults;
    effectsSettings.textureFilename = "sprites/particle_default.png";
    effectsSettings.blendMode = snowpulse::BlendMode::kNormal;
    effectsSettings.scaleStartEnd = snowpulse::Vector2(1.5f, 0.2f);
    effectsSettings.lifespanValueMode = snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin;
    effectsSettings.lifespanA = 0.8f;
    effectsSettings.lifespanB = 1.0f;
    effectsSettings.maxParticleCount = 10000;
    effectsSettings.emissionShape = snowpulse::ParticleSystemSettings::EmissionShape::kCircle;
    effectsSettings.emissionRectSize = snowpulse::Vector2(100.0f, 40.0f);
    effectsSettings.emissionRadius = 50.0f;
    effectsSettings.emissionRate = 40.0f;
    effectsSettings.emissionAngleValueMode = snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin;
    effectsSettings.emissionAngleA = 80.0f;
    effectsSettings.emissionAngleB = 100.0f;
    effectsSettings.speedValueMode = snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin;
    effectsSettings.speedA = 270.0f;
    effectsSettings.speedB = 350.0f;
    effectsSettings.acceleration = snowpulse::Vector2(0.0f, -1.0f) * 200.0f;
    effectsSettings.colorStart = snowpulse::Color::Red();
    effectsSettings.colorEnd = snowpulse::Color(1.0f, 0.6f, 0.0f, 0.0f);

    auto effectsGo = snowpulse::GameObject::Create("effectsGo");
    effectRenderer_ = snowpulse::ParticleEffectsRenderer::Create(effectsSettings);
    effectsGo->AddComponent(effectRenderer_);
    //effectsGo->GetTransform()->SetPosition(snowpulse::Vector2(-200.0f, 200.0f));
    AddChild(effectsGo);
}

void SceneMain::Update(float deltaTime) {
    Scene::Update(deltaTime);
    auto camPos = GetCamera()->GetPosition();
    background_->SetSize(snowpulse::Vector2(resolutionSize_.x, resolutionSize_.y));
    background_->GetTransform()->SetPosition(snowpulse::Vector2(camPos.x, camPos.y));

    if (snowpulse::Input::GetInstance()->GetPressed("x")) {
        GetApplication()->Close();
    }

#ifdef SPDEBUG
    std::cout << "SceneMain updating.. (deltaTime: " << deltaTime << ")" << std::endl;
#endif
}
}
