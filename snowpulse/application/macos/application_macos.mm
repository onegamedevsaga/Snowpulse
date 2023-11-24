#include "application_macos.h"

#import <Foundation/Foundation.h>

namespace snowpulse {
ApplicationMacOS::ApplicationMacOS() {
    platform_ = Platform::kMacOS;
    platformString_ = "macOS";
}

std::string ApplicationMacOS::GetPlatformPath(std::string filename) {
    NSString* filenameNSString = [NSString stringWithUTF8String:filename.c_str()];
    NSString* resourcePath = [[NSBundle mainBundle] pathForResource:filenameNSString ofType:nil];
    if (resourcePath == nil) {
        return filename;
    }
    return [resourcePath UTF8String];
}
}   // namespace snowpulse
