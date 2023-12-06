#include "application_ios.h"

namespace snowpulse {
ApplicationIOS::ApplicationIOS() {
    platform_ = Platform::kIOS;
    platformString_ = "iOS";
}

bool ApplicationIOS::Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize, void* view) {
    if (!ApplicationMetal::Initialize(resolutionSize, screenSize, view)) {
        return false;
    }

    directory_ = static_cast<DirectoryIOS*>(Directory::GetInstance());
    input_ = static_cast<InputIOS*>(Input::GetInstance());

#ifdef SPDEBUG
    std::cout << "ApplicationIOS initialized." << std::endl;
#endif
    return true;
}

void ApplicationIOS::Close() {
    //[NSApp terminate:self];
}

void ApplicationIOS::Shutdown() {
    ApplicationMetal::Shutdown();
    auto elapsed = appTimer_.GetElapsedInSeconds();
#ifdef SPDEBUG
    std::cout << "Application ran for " << elapsed << " seconds." << std::endl;
#endif
}
}   // namespace snowpulse
