#include "scene_main.h"

#include <iostream>

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

    auto actionGo = snowpulse::GameObject::Create("actionGo");
    auto actionPanel = ActionPanel::Create();
    actionGo->AddComponent(actionPanel);
    AddChild(actionGo);

    auto inspectorGo = snowpulse::GameObject::Create("inspectorGo");
    auto inspectorPanel = InspectorPanel::Create();
    inspectorGo->AddComponent(inspectorPanel);
    AddChild(inspectorGo);
}

void SceneMain::Update(float deltaTime) {
    Scene::Update(deltaTime);
    resolutionSize_ = GetApplication()->GetResolutionSize();
    background_->SetSize(snowpulse::Vector2(resolutionSize_.x, resolutionSize_.y));

    if (snowpulse::Input::GetInstance()->GetPressed("x")) {
        GetApplication()->Close();
    }
#ifdef SPDEBUG
    std::cout << "SceneMain updating.. (deltaTime: " << deltaTime << ")" << std::endl;
#endif
}
}
