#ifndef SNOWPULSE_APPLICATION_WINDOWS_DIRECTORYWINDOWS_H_
#define SNOWPULSE_APPLICATION_WINDOWS_DIRECTORYWINDOWS_H_

#include "../../common/singleton.h"
#include "../directory.h"

#include <string>

namespace snowpulse {
class SNOWPULSEAPI DirectoryWindows : public Directory, public Singleton<DirectoryWindows> {
    public:
        virtual ~DirectoryWindows();
        bool CreateDirectory(std::string path) override;
        std::string GetAssetsPath(std::string filename) override;
        std::string GetDefaultsPath(std::string filename) override;
        std::string GetApplicationSupportPath(std::string filename) override;
        std::string GetDocumentsPath(std::string filename) override;

    protected:
        friend class Singleton<DirectoryWindows>;
        DirectoryWindows();

        std::string GetResourcesPath();
        std::string GetWritablePath(std::string filename, std::string directory);
};
}   // namespace snowpulse
#endif
