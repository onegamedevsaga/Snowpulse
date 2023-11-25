#include "directory.h"

#ifdef SNOWPULSE_PLATFORM_WINDOWS
#include "windows/directory_windows.h"
#elif SNOWPULSE_PLATFORM_MACOS
#include "macos/directory_macos.h"
#elif SNOWPULSE_PLATFORM_IOS
//#include "ios/directory_ios.h"
#endif

namespace snowpulse {
Directory* Directory::GetInstance() {
#ifdef SNOWPULSE_PLATFORM_WINDOWS
    return DirectoryMacOS::GetInstance2();
#elif SNOWPULSE_PLATFORM_MACOS
    return DirectoryMacOS::GetInstance2();
#elif SNOWPULSE_PLATFORM_ANDROID
    
#elif SNOWPULSE_PLATFORM_IOS
    
#elif SNOWPULSE_PLATFORM_WEBGL
    
#endif
}

Directory::Directory() {
}

Directory::~Directory() {
}
}   // namespace snowpulse