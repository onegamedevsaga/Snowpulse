#ifndef SNOWPULSE_APPLICATION_METAL_APPLICATIONMETAL_H_
#define SNOWPULSE_APPLICATION_METAL_APPLICATIONMETAL_H_

#include "../application.h"

#include <string>
#include <memory>
#include <glad/glad.h>
//#include <GLFW/glfw3.h>

#include "../../defines.h"
#include "render_queue_metal.h"
#include "graphics_metal.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationMetal : public Application {
    public:
        virtual ~ApplicationMetal();
        virtual bool Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) override;
        virtual void Shutdown() override;
        Graphics* GetGraphics() const override { return graphics_.get(); }

        

    protected:
        ApplicationMetal();

        std::shared_ptr<GraphicsMetal> graphics_;
};
}   // namespace snowpulse

#endif
