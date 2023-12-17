#include "directory_windows.h"

#include <iostream>
#include <windows.h>
#include <shlobj.h>

#undef CreateDirectory

#include "../application.h"

namespace snowpulse {
DirectoryWindows::DirectoryWindows() {
}

DirectoryWindows::~DirectoryWindows() {
}

bool DirectoryWindows::CreateDirectory(std::string path) {
    return CreateDirectoryA(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
}

std::string DirectoryWindows::GetAssetsPath(std::string filename) {
    std::string path = Application::GetInstance()->GetAssetFolderPath();
    return path + filename;
}

std::string DirectoryWindows::GetDefaultsPath(std::string filename) {
    return "defaults\\" + filename;
}

std::string DirectoryWindows::GetApplicationSupportPath(std::string filename) {
    char path[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path);
    return std::string(path) + "\\YourGameName\\" + filename; // TODO: Get application name from Application
}

std::string DirectoryWindows::GetDocumentsPath(std::string filename) {
    char path[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path);
    return std::string(path) + "\\YourGameName\\" + filename; // TODO: Get application name from Application
}

std::string DirectoryWindows::GetResourcesPath() {
    /*if (IsDebuggerPresent()) {
        return "";
    }

    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    return GetPathFromFilename(std::string(path));*/
    return "";
}

std::string DirectoryWindows::GetWritablePath(std::string filename, std::string directory) {
    /*NSString *rootDir = [NSString stringWithUTF8String:directory.c_str()];
    size_t pos = filename.find_last_of("/\\");
    auto localDir = (pos != std::string::npos) ? filename.substr(0, pos + 1) : "";
    rootDir = [rootDir stringByAppendingPathComponent:[NSString stringWithUTF8String:localDir.c_str()]];
    if (!CreateDirectory([rootDir UTF8String])) {
        return filename;
    }
    auto actualFilename = (pos != std::string::npos) ? filename.substr(pos + 1, (int)filename.size() - pos) : filename;
    return [[rootDir stringByAppendingPathComponent:[NSString stringWithUTF8String:actualFilename.c_str()]] UTF8String];*/
    return "";
}
}   // namespace snowpulse
