#include "scene_level.h"

#include <iostream>

#include "components/imgui_test.h"

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

    auto atlasManager = snowpulse::Application::GetInstance()->GetAtlasManager();
    atlasManager->Load("atlas-load-test/atlasOutput.snowatlas");

    auto fontManager = snowpulse::Application::GetInstance()->GetFontManager();
    fontManager->Load("fonts/roboto/Roboto-Regular.ttf", 200.0f, snowpulse::PathType::kDefaults);

    go1_ = snowpulse::GameObject::Create("go1_");
    auto fontRenderer = snowpulse::FontRenderer::Create("fonts/roboto/Roboto-Regular.ttf", 60, snowpulse::PathType::kDefaults);
    //fontRenderer->SetText("ONE GAMEDEV SAGA");
    //fontRenderer->SetText("One Gamedev Saga");
    fontRenderer->SetText("Lorem Ipsum is simply dummy text of\nthe printing and typesetting industry.\nLorem Ipsum has been the industry's\nstandard dummy text ever since the\n1500s, when an unknown printer took\nLorem Ipsum is simply dummy text of\nthe printing and typesetting industry.\nLorem Ipsum has been the industry's\nstandard dummy text ever since the\n1500s, when an unknown printer took");
    fontRenderer->SetColor(snowpulse::Color(0.6f, 1.0f, 0.8f, 1.0f));
    fontRenderer->SetSortOrder(0);
    go1_->AddComponent(fontRenderer);
    //auto imguiTest = ImGUITest::Create();
    //go1_->AddComponent(imguiTest);
    AddChild(go1_);

    go1_->GetTransform()->SetLocalPosition(snowpulse::Vector2(-400.0f, 0.0f));

    /*snowpulse::ActionInterpolate::Create(0.0f, 360.0f*5.0f, 5.0f, snowpulse::Easing::kLinear)->OnUpdate([this](float value, snowpulse::ActionInterpolate* ai) {
        go1_->GetTransform()->SetRotation(value);
    })->OnComplete([this](snowpulse::Action* a) {
    })->Run();*/

    auto dup01 = snowpulse::GameObject::Create("dup01");
    auto sprRenderer = snowpulse::SpriteRenderer::Create("atlas-test/mobilephone.png");
    sprRenderer->SetColor(snowpulse::Color(1.0f, 1.0f, 1.0f, 1.0f));
    sprRenderer->SetSortOrder(0);
    dup01->AddComponent(sprRenderer);
    AddChild(dup01);

    dup01->GetTransform()->SetLocalPosition(snowpulse::Vector2(GetResolutionSize().x * -0.5f, GetResolutionSize().y * -0.5f));

    go2_ = snowpulse::GameObject::Create("go2_");
    sprRenderer = snowpulse::SpriteRenderer::Create("pixel-test.png", snowpulse::PathType::kAssets, snowpulse::TextureFiltering::kPoint);
    sprRenderer->SetSortOrder(2);
    sprRenderer->SetBlendMode(snowpulse::BlendMode::kNormal);
    go2_->AddComponent(sprRenderer);
    fontRenderer = snowpulse::FontRenderer::Create("fonts/roboto/Roboto-Regular.ttf", 60, snowpulse::PathType::kDefaults);
    //fontRenderer->SetText("ONE GAMEDEV SAGA");
    //fontRenderer->SetText("One Gamedev Saga");
    fontRenderer->SetText("Lorem Ipsum is simply dummy text of\nthe printing and typesetting industry.\nLorem Ipsum has been the industry's\nstandard dummy text ever since the\n1500s, when an unknown printer took\nLorem Ipsum is simply dummy text of\nthe printing and typesetting industry.\nLorem Ipsum has been the industry's\nstandard dummy text ever since the\n1500s, when an unknown printer took");
    fontRenderer->SetColor(snowpulse::Color(1.0f, 0.6f, 0.8f, 1.0f));
    fontRenderer->SetSortOrder(0);
    go2_->AddComponent(fontRenderer);
    go1_->AddChild(go2_);

    go2_->GetTransform()->SetLocalPosition(snowpulse::Vector2(-400.0f, 300.0f));
    go2_->GetTransform()->SetLocalScale(snowpulse::Vector2(1.0f, 1.0f));
    go2_->GetTransform()->SetRotation(0.0f);

    go3_ = snowpulse::GameObject::Create("go3_");
    auto spineRenderer = snowpulse::SpineRenderer::Create("hero/hero-pro.json", "hero/hero-pro.atlas");
    spineRenderer->SetSkin("weapon/sword");
    spineRenderer->PlayAnimation("run", true);
    spineRenderer->SetSortOrder(4);
    spineRenderer->SetIsPremultiplied(true);
    go3_->AddComponent(spineRenderer);
    AddChild(go3_);

    go3_->GetTransform()->SetPosition(snowpulse::Vector2(0.0f, (float)(GetResolutionSize().y) * -0.5f));
    go3_->GetTransform()->SetLocalScale(snowpulse::Vector2(-1.0f, 1.0f));
    
    auto sr = spineRenderer.get();
    go3_->GetTransform()->DoMoveX((float)(GetResolutionSize().x) * -0.5f + 450.0f, 2.0f, snowpulse::Easing::kLinear, false, [sr]() {
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

    tempGo->GetTransform()->SetLocalPosition(snowpulse::Vector2((float)(GetResolutionSize().x) * -0.5f + 300.0f, (float)(GetResolutionSize().y) * -0.5f + 100.0f));
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
    
    snowpulse::Vector2 mousePos = snowpulse::Input::GetInstance()->GetMousePositionOnScreen();
#ifdef SPDEBUG
    std::cout << "Mouse Position SCREEN (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
#endif
    
    mousePos = snowpulse::Input::GetInstance()->GetMousePositionOnWorld();
#ifdef SPDEBUG
    std::cout << "Mouse Position WORLD (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
#endif
    
    if (snowpulse::Input::GetInstance()->GetPressed("space")) {
        auto atlasManager = snowpulse::Application::GetInstance()->GetAtlasManager();
        std::vector<std::string> textureFilenames;
        textureFilenames.push_back("atlas-test/banner_01.png");
        textureFilenames.push_back("atlas-test/banner_02.png");
        textureFilenames.push_back("atlas-test/mobilephone.png");
        textureFilenames.push_back("atlas-test/opacity_test.png");
        atlasManager->Create(snowpulse::Vector2Int(1024, 1024), "atlasOutput", textureFilenames, snowpulse::PathType::kAssets, snowpulse::PathType::kDocuments, [](int progress) {
#ifdef SPDEBUG
            std::cout << "Atlas Manager: Progress at " << progress << "%." << std::endl;
#endif
        });
    }

    if (snowpulse::Input::GetInstance()->GetPressed("x")) {
        GetApplication()->Close();
    }
#ifdef SPDEBUG
    std::cout << "SceneLevel updating.. (deltaTime: " << deltaTime << ")" << std::endl;
#endif
}
}
