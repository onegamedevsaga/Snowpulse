#ifndef SNOWPULSE_APPLICATION_DIRECTORY_H_
#define SNOWPULSE_APPLICATION_DIRECTORY_H_

#include "../defines.h"

#include <string>

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

    protected:
        Directory();
};
}   // namespace snowpulse

#endif
