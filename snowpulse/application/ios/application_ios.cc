#include "application_ios.h"

#include "../directory.h"

namespace snowpulse {
ApplicationIOS::ApplicationIOS() {
    platform_ = Platform::kIOS;
    platformString_ = "iOS";
}

bool ApplicationIOS::Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) {
    if (!ApplicationMetal::Initialize(resolutionSize, screenSize)) {
        return false;
    }
    

    directory_ = static_cast<DirectoryIOS*>(Directory::GetInstance());
    return true;
}
}   // namespace snowpulse
