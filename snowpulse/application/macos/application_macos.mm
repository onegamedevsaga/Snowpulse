#include "application_macos.h"

#import <Foundation/Foundation.h>

#include "../directory.h"

namespace snowpulse {
ApplicationMacOS::ApplicationMacOS() {
    platform_ = Platform::kMacOS;
    platformString_ = "macOS";
}

bool ApplicationMacOS::Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) {
    if (!ApplicationOpenGL::Initialize(resolutionSize, screenSize)) {
        return false;
    }
    directory_ = static_cast<DirectoryMacOS*>(Directory::GetInstance());
    return true;
}
}   // namespace snowpulse
