#include "game.h"
#include "scene_level.h"

#include <iostream>

namespace game {
std::shared_ptr<Game> Game::Create() {
    auto game = std::shared_ptr<Game>(new Game());
    return game;
}

Game::Game() : snowpulse::Game() {
}

Game::~Game() {
#ifdef SPDEBUG
    std::cout << "Game library destroyed." << std::endl;
#endif
}

void Game::Shutdown() {
#ifdef SPDEBUG
    std::cout << "Game library shutdown." << std::endl;
#endif
    snowpulse::Game::Shutdown();
}

void Game::Initialize(snowpulse::Application* application) {
    snowpulse::Game::Initialize(application);
    auto scene = SceneLevel::Create(application);
    SetScene(scene);
#ifdef SPDEBUG
    std::cout << "Game library initialized." << std::endl;
#endif
}
}
