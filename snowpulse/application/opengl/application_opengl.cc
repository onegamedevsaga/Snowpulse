#include "application_opengl.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../timer.h"

namespace snowpulse {
ApplicationOpenGL::ApplicationOpenGL() : Application() {
    platform_ = Platform::kOpenGL;
    platformString_ = "OpenGL";
}

ApplicationOpenGL::~ApplicationOpenGL() {
#ifdef SPDEBUG
    std::cout << "ApplicationOpenGL destroyed." << std::endl;
#endif
}

bool ApplicationOpenGL::Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) {
    if (Application::Initialize(resolutionSize, screenSize)) {
#ifdef SPDEBUG
        std::cout << "Starting GLFW context, OpenGL 4.1" << std::endl;
#endif
        if (!glfwInit()) {
#ifdef SPDEBUG
            std::cerr << "Failed to initialize GLFW" << std::endl;
#endif
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);

        window_ = glfwCreateWindow(screenSize.x, screenSize.y, "Snowpulse", NULL, NULL);
        if (!window_) {
#ifdef SPDEBUG
            std::cerr << "Failed to create GLFW window" << std::endl;
#endif
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window_);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
#ifdef SPDEBUG
            std::cerr << "Failed to initialize GLAD" << std::endl;
#endif
            return false;
        }

        graphics_ = GraphicsOpenGL::Create();
        if (!graphics_ || !graphics_->Initialize(resolutionSize, screenSize)) {
#ifdef SPDEBUG
            std::cout << "ERROR: Application wasn't able to initialize graphics on this platform." << std::endl;
#endif
            return false;
        }

#ifdef SPDEBUG
        std::cout << "ApplicationOpenGL initialized." << std::endl;
#endif
        return true;
    }

    return false;
}

void ApplicationOpenGL::Close() {
    glfwSetWindowShouldClose(window_, true);
}

void ApplicationOpenGL::Shutdown() {
    if (game_) {
        game_->Shutdown();
    }
    if (graphics_) {
        graphics_->Shutdown();
    }
#ifdef SPDEBUG
    std::cout << "ApplicationOpenGL shutdown." << std::endl;
#endif
}
}   // namespace snowpulse
