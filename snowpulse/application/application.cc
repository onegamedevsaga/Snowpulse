#include "application.h"

#include <iostream>
#include <string>

#ifdef SNOWPULSE_PLATFORM_WINDOWS
#include "windows/application_windows.h"
#include "opengl/graphics_opengl.h"
#elif SNOWPULSE_PLATFORM_MACOS
#include "macos/application_macos.h"
#include "metal/graphics_metal.h"
#elif SNOWPULSE_PLATFORM_IOS
#include "ios/application_ios.h"
#include "metal/graphics_metal.h"
#endif

#include "../game/action_manager.h"

namespace snowpulse {
Application* Application::GetInstance() {
#ifdef SNOWPULSE_PLATFORM_WINDOWS
    return ApplicationWindows::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_MACOS
    return ApplicationMacOS::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_ANDROID
    
#elif SNOWPULSE_PLATFORM_IOS
    return ApplicationIOS::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_WEBGL
    
#endif
}

Application::Application() {
}

Application::~Application() {
#ifdef SPDEBUG
    std::cout << "Application destroyed." << std::endl;
#endif
}

bool Application::Initialize(const Vector2Int& resolution) {
    resolutionSize_ = resolution;
    actionManager_ = ActionManager::Create();
    appTimer_.Start();
    frameTimer_.Start();
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
