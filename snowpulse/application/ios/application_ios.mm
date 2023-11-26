#include "application_ios.h"

#import <OpenGLES/EAGL.h>

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
    EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if (!context) {
        // Handle the error here.
    }
    

    directory_ = static_cast<DirectoryIOS*>(Directory::GetInstance());
    return true;
}
}   // namespace snowpulse
