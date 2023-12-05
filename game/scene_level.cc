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
    snowpulse::Application::GetInstance()->GetPlatform();
    auto json = snowpulse::JsonUtils::LoadFile("settings.json", snowpulse::PathType::kAssets);
    if (json) {
#ifdef SPDEBUG
        std::cout << "JSON: " << (*json)["type"] << std::endl;
        std::cout << "JSON: " << json->dump(4) << std::endl;
#endif
    }
    
    (*json)["extraValue"] = 1234.0f;
    snowpulse::JsonUtils::SaveFile(json.get(), "global/settings.json", snowpulse::PathType::kApplicationSupport);
    
    auto json2 = snowpulse::JsonUtils::Create();
    (*json2)["levelNo"] = 1;
    (*json2)["levelName"] = "The Cathedral";
    snowpulse::JsonUtils::SaveFile(json2.get(), "levelSample.json");
    
    go1_ = snowpulse::GameObject::Create("go1_");
    auto sprRenderer = snowpulse::SpriteRenderer::Create("logo.png");
    sprRenderer->SetColor(snowpulse::Color(1.0f, 1.0f, 1.0f, 1.0f));
    sprRenderer->SetSortOrder(0);
    go1_->AddComponent(sprRenderer);
    AddChild(go1_);

    go1_->GetTransform()->SetLocalPosition(snowpulse::Vector2(1920.0f / 2.0f, 1080.0f / 2.0f));
    go1_->GetTransform()->SetLocalRotation(45.0f);
    //go1_->GetTransform()->SetScale(snowpulse::Vector3(0.3f, 0.3f, 1.0f));

    go2_ = snowpulse::GameObject::Create("go2_");
    sprRenderer = snowpulse::SpriteRenderer::Create("pixel-test.png", snowpulse::TextureFiltering::kPoint);
    sprRenderer->SetSortOrder(2);
    sprRenderer->SetBlendMode(snowpulse::BlendMode::kNormal);
    go2_->AddComponent(sprRenderer);
    go1_->AddChild(go2_);

    go2_->GetTransform()->SetLocalPosition(snowpulse::Vector2(300.0f, 300.0f));
    go2_->GetTransform()->SetLocalScale(snowpulse::Vector2(20.0f, 20.0f));
    go2_->GetTransform()->SetRotation(0.0f);

    go3_ = snowpulse::GameObject::Create("go3_");
    auto spineRenderer = snowpulse::SpineRenderer::Create("hero/hero-pro.json", "hero/hero-pro.atlas");
    spineRenderer->SetSkin("weapon/sword");
    spineRenderer->PlayAnimation("run", true);
    spineRenderer->SetSortOrder(4);
    spineRenderer->SetIsPremultiplied(true);
    go3_->AddComponent(spineRenderer);
    AddChild(go3_);

    go3_->GetTransform()->SetLocalPosition(snowpulse::Vector2(960.0f, 100.0f));
    go3_->GetTransform()->SetLocalScale(snowpulse::Vector2(-1.0f, 1.0f));
    
    auto sr = spineRenderer.get();
    go3_->GetTransform()->DoMoveX(340.0f, 2.0f, snowpulse::Easing::kLinear, false, [sr]() {
        sr->PlayAnimation("idle", true);
    });

    auto tempGo = snowpulse::GameObject::Create("tempGo");
    spineRenderer = snowpulse::SpineRenderer::Create("mix-and-match/mix-and-match-pro.json", "mix-and-match/mix-and-match-pro.atlas", snowpulse::TextureFiltering::kTrilinear);
    spineRenderer->SetSkin("full-skins/boy");
    spineRenderer->PlayAnimation("idle", true);
    spineRenderer->SetSortOrder(3);
    spineRenderer->SetIsPremultiplied(true);
    tempGo->AddComponent(spineRenderer);
    AddChild(tempGo);

    tempGo->GetTransform()->SetLocalPosition(snowpulse::Vector2(260.0f, 100.0f));
    tempGo->GetTransform()->SetLocalScale(snowpulse::Vector2(0.7f, 0.7f));

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
                //GetApplication()->Close();
            })->Run();
        });
    })->Run();
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
     
    auto touches = snowpulse::Input::GetInstance()->GetTouches();
    if (touches.size() > 0) {
        auto touch = touches[0];
        auto pos = touch.GetPosition();
        if (touch.GetPhase() == snowpulse::TouchPhase::kBegan) {
#ifdef SPDEBUG
            std::cout << "Touch Id:" << touch.GetId() << ": Began - (" << pos.x << ", " << pos.y << ")" << std::endl;
#endif
        }
        else if (touch.GetPhase() == snowpulse::TouchPhase::kMoved) {
#ifdef SPDEBUG
            std::cout << "Touch Id:" << touch.GetId() << ": Moved - (" << pos.x << ", " << pos.y << ")" << std::endl;
#endif
        }
        else if (touch.GetPhase() == snowpulse::TouchPhase::kEnded) {
#ifdef SPDEBUG
            std::cout << "Touch Id:" << touch.GetId() << ": Ended - (" << pos.x << ", " << pos.y << ")" << std::endl;
#endif
        }
    }
    
    snowpulse::Vector2 mousePos;// = snowpulse::Input::GetInstance()->GetPosition();
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
