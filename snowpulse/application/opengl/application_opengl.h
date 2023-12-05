#ifndef SNOWPULSE_APPLICATION_OPENGL_APPLICATIONOPENGL_H_
#define SNOWPULSE_APPLICATION_OPENGL_APPLICATIONOPENGL_H_

#include "../application.h"

#include <string>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../defines.h"
#include "render_queue_opengl.h"
#include "graphics_opengl.h"
#include "input_opengl.h"

namespace snowpulse {
class SNOWPULSEAPI ApplicationOpenGL : public Application {
    public:
        virtual ~ApplicationOpenGL();
        virtual bool Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) override;
        void Close() override;
        void Shutdown() override;
        Graphics* GetGraphics() const override { return graphics_.get(); }

        GLFWwindow* GetWindow() { return window_; }

    protected:
        ApplicationOpenGL();

        GLFWwindow* window_;
        std::shared_ptr<GraphicsOpenGL> graphics_;
};
}   // namespace snowpulse

#endif
