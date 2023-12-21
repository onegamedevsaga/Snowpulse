#ifndef SNOWPULSE_APPLICATION_DIRECTORY_H_
#define SNOWPULSE_APPLICATION_DIRECTORY_H_

#include "../defines.h"

#include <string>

#include "../common/path_type.h"

namespace snowpulse {
class SNOWPULSEAPI Directory {
    public:
        static Directory* GetInstance();

        virtual ~Directory();
        virtual bool CreateDirectory(std::string path) = 0;
        virtual std::string GetAssetsPath(std::string filename) = 0;
        virtual std::string GetDefaultsPath(std::string filename) = 0;
        virtual std::string GetApplicationSupportPath(std::string filename) = 0;
        virtual std::string GetDocumentsPath(std::string filename) = 0;

        std::string GetFullFilename(std::string filename, PathType pathType);
        std::string GetPathFromFilename(std::string pathWithFilename);
        std::string GetFilenameFromPath(std::string filenamePath, bool removeExtension=false);

    protected:
        Directory();
};
}   // namespace snowpulse

#endif
