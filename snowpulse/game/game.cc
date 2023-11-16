#include "game.h"

#include <iostream>

#include "../application/application.h"

namespace snowpulse {
Game::Game() : Node("game") {
    scene_ = SPNULL;
#ifdef SPDEBUG
    std::cout << "Game initialized." << std::endl;
#endif
}

Game::~Game() {
#ifdef SPDEBUG
    std::cout << "Game destroyed." << std::endl;
#endif
}

void Game::Shutdown() {
    if (scene_.get()) {
        scene_->Shutdown();
    }
#ifdef SPDEBUG
    std::cout << "Game shutdown." << std::endl;
#endif
}

void Game::Initialize(Application* application) {
    application_ = application;
}

void Game::Start() {
}

void Game::SetScene(std::shared_ptr<Scene> scene) {
    if (scene_.get()) {
        scene_->Shutdown();
    }

    scene_ = scene;
    scene_->SetGame(this);
    scene_->Start();
}

void Game::UpdateScene(float deltaTime) {
    if (scene_) {
        scene_->Update(deltaTime);
    }
}

void Game::DrawScene(Graphics* graphics) {
    if (scene_) {
        scene_->Draw(graphics, Matrix4x4());
    }
}
}   // namespace snowpulse
