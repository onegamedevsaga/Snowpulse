#include "application_windows.h"

namespace snowpulse {
    ApplicationWindows::ApplicationWindows() {
        platform_ = Platform::kWindows;
        platformString_ = "Windows";
    }

    bool ApplicationWindows::Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) {
        if (!ApplicationOpenGL::Initialize(resolutionSize, screenSize)) {
            return false;
        }

        directory_ = static_cast<DirectoryWindows*>(Directory::GetInstance());
        input_ = static_cast<InputWindows*>(Input::GetInstance());

#ifdef SPDEBUG
        std::cout << "ApplicationWindows initialized." << std::endl;
#endif
        return true;
    }

    void ApplicationWindows::Close() {
        glfwSetWindowShouldClose(window_, true);
    }

    void ApplicationWindows::Shutdown() {
        ApplicationOpenGL::Shutdown();
        auto elapsed = appTimer_.GetElapsedInSeconds();
#ifdef SPDEBUG
        std::cout << "Application ran for " << elapsed << " seconds." << std::endl;
#endif
    }
}   // namespace snowpulse

