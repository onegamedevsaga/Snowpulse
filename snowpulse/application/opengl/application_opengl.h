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

namespace snowpulse {
class SNOWPULSEAPI ApplicationOpenGL : public Application {
    public:
        virtual ~ApplicationOpenGL();
        virtual bool Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) override;
        virtual void SetScreenSize(const Vector2Int& screenSize) override;
        virtual void Shutdown() override;
        virtual void SetScreenScaleFactor(float scaleFactor) override;

        virtual void Run(); // TEMP
        virtual void RunFrame();

        Graphics* GetGraphics() const override { return graphics_.get(); }
        GLFWwindow* GetWindow() { return window_; }

    protected:
        ApplicationOpenGL();

        std::shared_ptr<GraphicsOpenGL> graphics_;
        GLFWwindow* window_;
};
}   // namespace snowpulse

#endif
