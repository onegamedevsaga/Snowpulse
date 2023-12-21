#include "directory.h"

#ifdef SNOWPULSE_PLATFORM_WINDOWS
#include "windows/directory_windows.h"
#elif SNOWPULSE_PLATFORM_MACOS
#include "macos/directory_macos.h"
#elif SNOWPULSE_PLATFORM_IOS
#include "ios/directory_ios.h"
#endif

#include "../common/path_type.h"

namespace snowpulse {
Directory* Directory::GetInstance() {
#ifdef SNOWPULSE_PLATFORM_WINDOWS
    return DirectoryWindows::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_MACOS
    return DirectoryMacOS::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_ANDROID
    
#elif SNOWPULSE_PLATFORM_IOS
    return DirectoryIOS::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_WEBGL
    
#endif
}

Directory::Directory() {
}

Directory::~Directory() {
}

std::string Directory::GetFullFilename(std::string filename, PathType pathType) {
    std::string fullFilename = "";
    switch (pathType) {
        case PathType::kAssets:
            fullFilename = GetAssetsPath(filename);
            break;
        case PathType::kDefaults:
            fullFilename = GetDefaultsPath(filename);
            break;
        case PathType::kApplicationSupport:
            fullFilename = GetApplicationSupportPath(filename);
            break;
        case PathType::kDocuments:
            fullFilename = GetDocumentsPath(filename);
            break;
        case PathType::kRaw:
            fullFilename = filename;
        default:
            break;
    }
    return fullFilename;
}

std::string Directory::GetPathFromFilename(std::string pathWithFilename) {
    size_t pos = pathWithFilename.find_last_of("/\\");
    return (pos != std::string::npos) ? pathWithFilename.substr(0, pos + 1) : "";
}

std::string Directory::GetFilenameFromPath(std::string filenamePath, bool removeExtension) {
    size_t pos = filenamePath.find_last_of("/\\");
    auto filename = (pos != std::string::npos) ? filenamePath.substr(pos + 1, filenamePath.size() - pos) : filenamePath;
    if (removeExtension) {
        pos = filename.find_last_of(".");
        filename = (pos != std::string::npos) ? filename.substr(0, pos) : filename;
    }
    return filename;
}
}   // namespace snowpulse
