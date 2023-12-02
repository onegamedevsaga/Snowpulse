#ifndef SNOWPULSE_APPLICATION_MACOS_APPLICATIONMACOS_H_
#define SNOWPULSE_APPLICATION_MACOS_APPLICATIONMACOS_H_

#include "../../common/singleton.h"
#include "../opengl/application_opengl.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "directory_macos.h"
#include "input_macos.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationMacOS : public ApplicationOpenGL, public Singleton<ApplicationMacOS> {
    public:
        virtual bool Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize, void* device);
        void Close() override;
        void Shutdown() override;

        void Run();

        Vector2Int GetScreenSize() { return screenSize_; }

    protected:
        friend class Singleton<ApplicationMacOS>;
        ApplicationMacOS();

        Vector2Int screenSize_;
        DirectoryMacOS* directory_;
        InputMacOS* input_;
};
}   // namespace snowpulse

#endif
