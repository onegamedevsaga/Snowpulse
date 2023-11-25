#include "application_ios.h"

#import <Foundation/Foundation.h>

#include "../directory.h"

namespace snowpulse {
ApplicationIOS::ApplicationIOS() {
    platform_ = Platform::kIOS;
    platformString_ = "iOS";
}

bool ApplicationIOS::Initialize() {
    if (!ApplicationOpenGLES::Initialize()) {
        return false;
    }
    directory_ = static_cast<DirectoryIOS*>(Directory::GetInstance());
    return true;
}
}   // namespace snowpulse
