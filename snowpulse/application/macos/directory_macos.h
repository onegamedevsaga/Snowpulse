#ifndef SNOWPULSE_APPLICATION_MACOS_DIRECTORYMACOS_H_
#define SNOWPULSE_APPLICATION_MACOS_DIRECTORYMACOS_H_

#include "../../common/singleton.h"
#include "../directory.h"

#include <string>

namespace snowpulse {
class SNOWPULSEAPI DirectoryMacOS : public Directory, public Singleton<DirectoryMacOS> {
    public:
        virtual ~DirectoryMacOS();
        bool CreateDirectory(std::string path) override;
        std::string GetAssetsPath(std::string filename) override;
        std::string GetDefaultsPath(std::string filename) override;
        std::string GetApplicationSupportPath(std::string filename) override;
        std::string GetDocumentsPath(std::string filename) override;

    protected:
        friend class Singleton<DirectoryMacOS>;
        DirectoryMacOS();

        std::string GetResourcesPath(std::string filename);
        std::string GetWritablePath(std::string filename, std::string directory);
};
}   // namespace snowpulse
#endif
