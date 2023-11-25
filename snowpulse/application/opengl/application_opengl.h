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
        virtual bool Initialize()  override;
        void Run() override;
        void Close() override;
        void Shutdown() override;
        Graphics* GetGraphics() const override { return graphics_.get(); }

    protected:
        ApplicationOpenGL();

        InputOpenGL* input_;
        std::shared_ptr<GraphicsOpenGL> graphics_;
        std::shared_ptr<RenderQueueOpenGL> renderQueue_;
};
}   // namespace snowpulse

#endif
