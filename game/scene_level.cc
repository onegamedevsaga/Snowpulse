#include "scene_level.h"

#include <iostream>

namespace game {
std::shared_ptr<SceneLevel> SceneLevel::Create(snowpulse::Application* application) {
    auto scene = std::shared_ptr<SceneLevel>(new SceneLevel());
    scene->Initialize(application);
    return scene;
}

SceneLevel::SceneLevel() : snowpulse::Scene() {
}

SceneLevel::~SceneLevel() {
#ifdef SPDEBUG
    std::cout << "SceneLevel destroyed." << std::endl;
#endif
}

void SceneLevel::Initialize(snowpulse::Application* application) {
    Scene::Initialize(application);
    //application->SetName("Blah blah");
#ifdef SPDEBUG
    std::cout << "SceneLevel initialized." << std::endl;
#endif
}

void SceneLevel::Shutdown() {
    go1_.reset();
    go2_.reset();
    go3_.reset();
#ifdef SPDEBUG
    std::cout << "SceneLevel shutdown." << std::endl;
#endif
    snowpulse::Scene::Shutdown();
}

void SceneLevel::Start() {
    auto json = snowpulse::JsonUtils::LoadFile("settings.json");
    if (json) {
#ifdef SPDEBUG
        std::cout << "JSON: " << (*json)["type"] << std::endl;
        std::cout << "JSON: " << json->dump(4) << std::endl;
#endif
    }
    
    (*json)["extraValue"] = 1234.0f;
    snowpulse::JsonUtils::SaveFile(json.get(), "settings.json");
    
    auto json2 = snowpulse::JsonUtils::Create();
    (*json2)["levelNo"] = 1;
    (*json2)["levelName"] = "The Cathedral";
    snowpulse::JsonUtils::SaveFile(json2.get(), "levelSample.json");
    
    go1_ = snowpulse::GameObject::Create("go1_");
    auto sprRenderer = snowpulse::SpriteRenderer::Create("logo.png");
    sprRenderer->SetColor(snowpulse::Color::Red());
    sprRenderer->SetSortOrder(0);
    go1_->AddComponent(sprRenderer);
    AddChild(go1_);

    go1_->GetTransform()->SetLocalPosition(snowpulse::Vector2(1920.0f / 2.0f, 1080.0f / 2.0f));
    go1_->GetTransform()->SetLocalRotation(45.0f);
    //go1_->GetTransform()->SetScale(snowpulse::Vector3(0.3f, 0.3f, 1.0f));

    go2_ = snowpulse::GameObject::Create("go2_");
    sprRenderer = snowpulse::SpriteRenderer::Create("logo.png");
    sprRenderer->SetSortOrder(2);
    sprRenderer->SetBlendMode(snowpulse::BlendMode::kAdditive);
    go2_->AddComponent(sprRenderer);
    go1_->AddChild(go2_);

    go2_->GetTransform()->SetLocalPosition(snowpulse::Vector2(300.0f, 300.0f));
    go2_->GetTransform()->SetLocalScale(snowpulse::Vector2(1.0f, 1.0f));
    go2_->GetTransform()->SetLocalRotation(0.0f);

    go3_ = snowpulse::GameObject::Create("go3_");
    //auto spineRenderer = snowpulse::SpineRenderer::Create("spine-test/spine-test.json", "spine-test/spine-test.atlas");
    auto spineRenderer = snowpulse::SpineRenderer::Create("hero/hero.json", "hero/hero.atlas");
    //auto spineRenderer = snowpulse::SpineRenderer::Create("level_complete/level_complete.json", "level_complete/level_complete.atlas");
    spineRenderer->SetSortOrder(3);
    go3_->AddComponent(spineRenderer);
    AddChild(go3_);

    go3_->GetTransform()->SetLocalPosition(snowpulse::Vector2(960.0f, 540.0f));
    //go3_->GetTransform()->SetRotation(90.0f);

    snowpulse::ActionWait::Create(4.0f)->OnComplete([this](snowpulse::Action* a) {
        go2_->GetTransform()->DoMoveX(0.0f, 1.0f, snowpulse::Easing::kBounceOut, false, [this]() {
#ifdef SPDEBUG
            std::cout << "DoMove of " << go2_->GetName() << " is complete." << std::endl;
#endif
            snowpulse::ActionInterpolate::Create(0.0f, 10.0f, 1.0f, snowpulse::Easing::kLinear)->OnUpdate([](float value, snowpulse::ActionInterpolate* ai) {
#ifdef SPDEBUG
                std::cout << "Tick: " << value << std::endl;
#endif
            })->OnComplete([this](snowpulse::Action* a) {
                GetApplication()->Close();
            })->Run();
        });
    });//->Run();
}

void SceneLevel::Update(float deltaTime) {
    Scene::Update(deltaTime);
    auto inc = snowpulse::Vector3(100.2f * deltaTime, 100.2f * deltaTime, 0.0f);
    //go1_->GetTransform()->SetPosition(go1_->GetTransform()->GetPosition() + inc);

    auto rot = 360.0f * deltaTime;
    //go1_->GetTransform()->SetLocalRotation(go1_->GetTransform()->GetLocalRotation() + rot);

    auto scl = snowpulse::Vector2(0.2f * deltaTime, 0.0f * deltaTime);
    //go1_->GetTransform()->SetLocalScale(go1_->GetTransform()->GetLocalScale() + scl);

    rot = 180.0f * deltaTime;
    //go2_->GetTransform()->SetLocalRotation(go2_->GetTransform()->GetLocalRotation() + rot);
    
    auto camPos = GetCamera()->GetPosition();
    auto camMovement = 100.0f * deltaTime;
    //GetCamera()->SetPosition(snowpulse::Vector2(camPos.x + camMovement, camPos.y + camMovement));
     
    
    auto mousePos = snowpulse::Input::GetInstance()->GetInputPosition();
#ifdef SPDEBUG
    std::cout << "Mouse Position (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
#endif
    
    if (snowpulse::Input::GetInstance()->GetPressed("mouse_middle")) {
        if (mousePos.x > 1920/2 && mousePos.y > 1080 / 2) {
            //GetApplication()->Close();
            go2_->AddChild(go3_);
        }
    }
    
    if (snowpulse::Input::GetInstance()->GetPressed("x")) {
        GetApplication()->Close();
    }
#ifdef SPDEBUG
    std::cout << "SceneLevel updating.. (deltaTime: " << deltaTime << ")" << std::endl;
#endif
}
}
