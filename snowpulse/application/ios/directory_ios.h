#ifndef SNOWPULSE_APPLICATION_IOS_DIRECTORYIOS_H_
#define SNOWPULSE_APPLICATION_IOS_DIRECTORYIOS_H_

#include "../../common/singleton.h"
#include "../directory.h"

#include <string>

namespace snowpulse {
class SNOWPULSEAPI DirectoryIOS : public Directory, public Singleton<DirectoryIOS> {
    public:
        virtual ~DirectoryIOS();
        bool CreateDirectory(std::string path) override;
        std::string GetAssetsPath(std::string filename) override;
        std::string GetDefaultsPath(std::string filename) override;
        std::string GetApplicationSupportPath(std::string filename) override;
        std::string GetDocumentsPath(std::string filename) override;

    protected:
        friend class Singleton<DirectoryIOS>;
        DirectoryIOS();

        std::string GetResourcesPath(std::string filename);
        std::string GetWritablePath(std::string filename, std::string directory);
};
}   // namespace snowpulse

#endif
