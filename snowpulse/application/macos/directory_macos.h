#ifndef SNOWPULSE_APPLICATION_MACOS_DIRECTORYMACOS_H_
#define SNOWPULSE_APPLICATION_MACOS_DIRECTORYMACOS_H_

#include "../../common/singleton.h"
#include "../directory.h"

#include <string>

namespace snowpulse {
class SNOWPULSEAPI DirectoryMacOS : public Directory, public Singleton<DirectoryMacOS> {
    public:
        virtual ~DirectoryMacOS();
        std::string GetAssetsPath(std::string filename);

    protected:
        friend class Singleton<DirectoryMacOS>;
        DirectoryMacOS();
};
}   // namespace snowpulse

#endif
