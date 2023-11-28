#include "application_metal.h"

#include <iostream>

#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include "../timer.h"
#include "../input.h"

#if SNOWPULSE_PLATFORM_MACOS
#include "../macos/application_macos.h"
#elif SNOWPULSE_PLATFORM_IOS
#include "../ios/application_ios.h"
#endif

namespace snowpulse {
ApplicationMetal::ApplicationMetal() : Application() {
    platform_ = Platform::kMetal;
    platformString_ = "Metal";
}

ApplicationMetal::~ApplicationMetal() {
#ifdef SPDEBUG
    std::cout << "ApplicationMetal destroyed." << std::endl;
#endif
}

bool ApplicationMetal::Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) {
    if (!Application::Initialize(resolutionSize, screenSize)) {
        
        return false;
    }
    return true;
}

void ApplicationMetal::Shutdown() {
    if (game_) {
        game_->Shutdown();
    }
    if (graphics_) {
        graphics_->Shutdown();
    }
#ifdef SPDEBUG
    std::cout << "ApplicationMetal shutdown." << std::endl;
#endif
}
}   // namespace snowpulse
