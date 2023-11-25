#ifndef SNOWPULSE_APPLICATION_OPENGLES_APPLICATIONOPENGLES_H_
#define SNOWPULSE_APPLICATION_OPENGLES_APPLICATIONOPENGLES_H_

#include "../application.h"

#include <string>
#include <memory>

#include "../../defines.h"
#include "render_queue_opengles.h"
#include "graphics_opengles.h"
#include "../ios/input_ios.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationOpenGLES : public Application {
    public:
        virtual ~ApplicationOpenGLES();
        virtual bool Initialize()  override;
        void Run() override;
        void Close() override;
        void Shutdown() override;
        Graphics* GetGraphics() const override { return graphics_.get(); }

    protected:
        ApplicationOpenGLES();

        InputIOS* input_;
        std::shared_ptr<GraphicsOpenGLES> graphics_;
        std::shared_ptr<RenderQueueOpenGLES> renderQueue_;
};
}   // namespace snowpulse

#endif
