#include "application_macos.h"

#import <Foundation/Foundation.h>

#include "../directory.h"

namespace snowpulse {
ApplicationMacOS::ApplicationMacOS() {
    platform_ = Platform::kMacOS;
    platformString_ = "macOS";
}

bool ApplicationMacOS::Initialize() {
    if (!ApplicationOpenGL::Initialize()) {
        return false;
    }
    directory_ = static_cast<DirectoryMacOS*>(Directory::GetInstance());
    return true;
}
}   // namespace snowpulse
