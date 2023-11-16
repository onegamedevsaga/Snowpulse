#include "application.h"

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef SNOWPULSE_PLATFORM_WINDOWS
#include "opengl/graphics_opengl.h"
#elif SNOWPULSE_PLATFORM_MACOS
#include "opengl/graphics_opengl.h"
#endif

#include "timer.h"
#include "../game/action_manager.h"

namespace snowpulse {
Application::Application() {
}

Application::~Application() {
#ifdef SPDEBUG
    std::cout << "Application destroyed." << std::endl;
#endif
}

bool Application::Initialize() {
    camera_ = Camera::Create();
    actionManager_ = ActionManager::Create();
    return true;
}

void Application::SetGame(Game* game) {
    game_ = game;
}

float Application::GetDeltaTime(Timer& timer) {
    auto elapsed = timer.GetElapsedInSeconds();
    if (elapsed > 0.0333f) {
        elapsed = 0.0333f;
    }
    timer.Start();
    return elapsed;
}
}   // namespace snowpulse
