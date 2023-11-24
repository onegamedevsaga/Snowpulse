#ifndef SNOWPULSE_APPLICATION_MACOS_APPLICATIONMACOS_H_
#define SNOWPULSE_APPLICATION_MACOS_APPLICATIONMACOS_H_

#include "../../common/singleton.h"
#include "../opengl/application_opengl.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationMacOS : public ApplicationOpenGL, public Singleton<ApplicationMacOS> {
    public:
        std::string GetPlatformPath(std::string filename);

    protected:
        friend class Singleton<ApplicationMacOS>;
        ApplicationMacOS();
};
}   // namespace snowpulse

#endif
