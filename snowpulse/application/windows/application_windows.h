#ifndef SNOWPULSE_APPLICATION_WINDOWS_APPLICATIONWINDOWS_H_
#define SNOWPULSE_APPLICATION_WINDOWS_APPLICATIONWINDOWS_H_

#include "../opengl/application_opengl.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationWindows : public ApplicationOpenGL {
    public:
        static std::shared_ptr<ApplicationWindows> Create();

    protected:
        ApplicationWindows();
};
}   // namespace snowpulse

#endif
