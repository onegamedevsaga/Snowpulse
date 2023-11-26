#ifndef SNOWPULSE_APPLICATION_METAL_APPLICATIONMETAL_H_
#define SNOWPULSE_APPLICATION_METAL_APPLICATIONMETAL_H_

#include "../application.h"

#include <string>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../defines.h"
#include "render_queue_metal.h"
#include "graphics_metal.h"
#include "../macos/input_macos.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationMetal : public Application {
    public:
        virtual ~ApplicationMetal();
        virtual bool Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) override;
        void Run() override;
        void Close() override;
        void Shutdown() override;
        Graphics* GetGraphics() const override { return graphics_.get(); }

        GLFWwindow* GetWindow() { return window_; }

    protected:
        ApplicationMetal();

        GLFWwindow* window_;
        InputMacOS* input_;
        std::shared_ptr<GraphicsMetal> graphics_;
};
}   // namespace snowpulse

#endif
