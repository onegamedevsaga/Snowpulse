#ifndef SNOWPULSE_APPLICATION_MACOS_APPLICATIONMACOS_H_
#define SNOWPULSE_APPLICATION_MACOS_APPLICATIONMACOS_H_

#include "../opengl/application_opengl.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationMacOS : public ApplicationOpenGL {
    public:
        static std::shared_ptr<ApplicationMacOS> Create();

    protected:
        ApplicationMacOS();
};
}   // namespace snowpulse

#endif
