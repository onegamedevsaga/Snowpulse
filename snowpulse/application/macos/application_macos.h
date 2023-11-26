#ifndef SNOWPULSE_APPLICATION_MACOS_APPLICATIONMACOS_H_
#define SNOWPULSE_APPLICATION_MACOS_APPLICATIONMACOS_H_

#include "../../common/singleton.h"
#include "../opengl/application_opengl.h"

#include "directory_macos.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationMacOS : public ApplicationOpenGL, public Singleton<ApplicationMacOS> {
    public:
        virtual bool Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) override;

    protected:
        friend class Singleton<ApplicationMacOS>;
        ApplicationMacOS();

        DirectoryMacOS* directory_;
};
}   // namespace snowpulse

#endif
