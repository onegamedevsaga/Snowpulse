#include "directory_macos.h"

#include <iostream>
#import <Foundation/Foundation.h>

namespace snowpulse {
DirectoryMacOS::DirectoryMacOS() {
}

DirectoryMacOS::~DirectoryMacOS() {
}

bool DirectoryMacOS::CreateDirectory(std::string path) {
    NSString* pathNSString = [NSString stringWithUTF8String:path.c_str()];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:pathNSString]) {
        NSError *error = nil;
        [fileManager createDirectoryAtPath:pathNSString withIntermediateDirectories:YES attributes:nil error:&error];
        if (error) {
            NSLog(@"Error creating directory: %@", error);
#ifdef SPDEBUG
            std::cerr << "Error: Creating (" << [pathNSString UTF8String] << ") directory." << std::endl;
#endif
        }
    }
    return true;
}

std::string DirectoryMacOS::GetAssetsPath(std::string filename) {
    return GetResourcesPath("assets/" + filename);
}

std::string DirectoryMacOS::GetDefaultsPath(std::string filename) {
    return GetResourcesPath("defaults/" + filename);
}

std::string DirectoryMacOS::GetApplicationSupportPath(std::string filename) {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    return GetWritablePath(filename, [[paths firstObject] UTF8String]);
}

std::string DirectoryMacOS::GetDocumentsPath(std::string filename) {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    return GetWritablePath(filename, [[paths firstObject] UTF8String]);
}

std::string DirectoryMacOS::GetResourcesPath(std::string filename) {
    NSString* filenameNSString = [NSString stringWithUTF8String:filename.c_str()];
    NSString* resourcePath = [[NSBundle mainBundle] pathForResource:filenameNSString ofType:nil];
    if (resourcePath == nil) {
        return filename;
    }
    return [resourcePath UTF8String];
}

std::string DirectoryMacOS::GetWritablePath(std::string filename, std::string directory) {
    NSString *rootDir = [NSString stringWithUTF8String:directory.c_str()];
    size_t pos = filename.find_last_of("/\\");
    auto localDir = (pos != std::string::npos) ? filename.substr(0, pos + 1) : "";
    rootDir = [rootDir stringByAppendingPathComponent:[NSString stringWithUTF8String:localDir.c_str()]];
    if (!CreateDirectory([rootDir UTF8String])) {
        return filename;
    }
    auto actualFilename = (pos != std::string::npos) ? filename.substr(pos + 1, (int)filename.size() - pos) : filename;
    return [[rootDir stringByAppendingPathComponent:[NSString stringWithUTF8String:actualFilename.c_str()]] UTF8String];
}
}   // namespace snowpulse
