#include "application_macos.h"

#import <AppKit/AppKit.h>

namespace snowpulse {
ApplicationMacOS::ApplicationMacOS() {
    platform_ = Platform::kMacOS;
    platformString_ = "macOS";
}

bool ApplicationMacOS::Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize, void* view) {
    if (!ApplicationMetal::Initialize(resolutionSize, screenSize, view)) {
        return false;
    }

    directory_ = static_cast<DirectoryMacOS*>(Directory::GetInstance());
    input_ = static_cast<InputMacOS*>(Input::GetInstance());

#ifdef SPDEBUG
    std::cout << "ApplicationMacOS initialized." << std::endl;
#endif
    return true;
}

void ApplicationMacOS::Close() {
    [NSApp terminate:nil];
}

void ApplicationMacOS::Shutdown() {
    ApplicationMetal::Shutdown();
    auto elapsed = appTimer_.GetElapsedInSeconds();
#ifdef SPDEBUG
    std::cout << "Application ran for " << elapsed << " seconds." << std::endl;
#endif
}
}   // namespace snowpulse
