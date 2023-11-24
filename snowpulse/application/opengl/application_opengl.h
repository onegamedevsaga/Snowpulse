#ifndef SNOWPULSE_APPLICATION_OPENGL_APPLICATIONOPENGL_H_
#define SNOWPULSE_APPLICATION_OPENGL_APPLICATIONOPENGL_H_

#include "../application.h"

#include <string>
#include <memory>

#include "../../defines.h"
#include "render_queue_opengl.h"
#include "graphics_opengl.h"
#include "input_opengl.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationOpenGL : public Application {
    public:
        virtual ~ApplicationOpenGL();

        void Run();
        void Close();
        void Shutdown();
        Graphics* GetGraphics() const { return graphics_.get(); }

    protected:
        ApplicationOpenGL();
        bool Initialize();

        InputOpenGL* input_;
        std::shared_ptr<GraphicsOpenGL> graphics_;
        std::shared_ptr<RenderQueueOpenGL> renderQueue_;
};
}   // namespace snowpulse

#endif
