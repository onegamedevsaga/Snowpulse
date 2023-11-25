#include "directory_macos.h"

#import <Foundation/Foundation.h>

namespace snowpulse {
DirectoryMacOS::DirectoryMacOS() {
}

DirectoryMacOS::~DirectoryMacOS() {
}

std::string DirectoryMacOS::GetAssetsPath(std::string filename) {
    NSString* filenameNSString = [NSString stringWithUTF8String:filename.c_str()];
    NSString* resourcePath = [[NSBundle mainBundle] pathForResource:filenameNSString ofType:nil];
    if (resourcePath == nil) {
        return filename;
    }
    return [resourcePath UTF8String];
}
}   // namespace snowpulse
