#ifndef SNOWPULSE_APPLICATION_WINDOWS_INPUTWINDOWS_H_
#define SNOWPULSE_APPLICATION_WINDOWS_INPUTWINDOWS_H_

#include "../../common/singleton.h"
#include "../input.h"

#include <string>
#include <map>
#include <GLFW/glfw3.h>

#include "../../common/vector2.h"
#include "../../common/vector2int.h"

namespace snowpulse {
class SNOWPULSEAPI InputWindows : public Input, public Singleton<InputWindows> {
    public:
        virtual ~InputWindows();

        void ProcessInputs(const Vector2Int& resolutionSize, const Vector2Int& screenSize, GLFWwindow* window);
        void ProcessInputs(const Vector2Int& resolutionSize, const Vector2Int& screenSize, Vector2 mousePosition);
        void ProcessInputs(const Vector2Int& resolutionSize, const Vector2Int& screenSize, Vector2 mousePosition, int mouseButton, bool isMouseDown);
        void ProcessInputs(unsigned int key, bool isKeyDown);
        void ProcessInputs(std::string characters);
        void ProcessInputs(const Vector2& scrollDelta);

        void ClearLastFrameData() override;
        bool GetPressed(std::string key) override;
        bool GetDown(std::string key) override;
        bool GetReleased(std::string key) override;

    protected:
        friend class Singleton<InputWindows>;
        InputWindows();
};
}   // namespace snowpulse
#endif
