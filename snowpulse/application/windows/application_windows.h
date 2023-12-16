#ifndef SNOWPULSE_APPLICATION_WINDOWS_APPLICATIONWINDOWS_H_
#define SNOWPULSE_APPLICATION_WINDOWS_APPLICATIONWINDOWS_H_

#include "../../common/singleton.h"
#include "../opengl/application_opengl.h"

#include "directory_windows.h"
#include "../windows/input_windows.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationWindows : public ApplicationOpenGL, public Singleton<ApplicationWindows> {
    public:
        virtual bool Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) override;
        void Close() override;
        void Shutdown() override;

    protected:
        friend class Singleton<ApplicationWindows>;
        ApplicationWindows();

        DirectoryWindows* directory_;
        InputWindows* input_;
};
}   // namespace snowpulse

#endif
