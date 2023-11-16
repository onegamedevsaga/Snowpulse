#ifndef SNOWPULSE_APPLICATION_OPENGL_INPUTOPENGL_H_
#define SNOWPULSE_APPLICATION_OPENGL_INPUTOPENGL_H_

#include "../input.h"

#include <memory>
#include <string>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../common/vector2.h"
#include "graphics_opengl.h"

namespace snowpulse {
class SNOWPULSEAPI InputOpenGL : public Input {
    public:
        static std::shared_ptr<InputOpenGL> Create();

        virtual ~InputOpenGL();

        void ProcessInputs(GraphicsOpenGL* graphics);
        bool GetPressed(std::string key);
        bool GetReleased(std::string key);
        Vector2 GetInputPosition() { return inputPosition_; }

    protected:
        InputOpenGL();

        Vector2 inputPosition_;
        std::map<unsigned int, std::string> keyMap_;
        std::map<unsigned int, std::string> mouseButtonMap_;
        std::map<std::string, bool> pressedInputs_;
        std::map<std::string, bool> releasedInputs_;

        std::map<std::string, bool> canBePressedInputs_;
};
}   // namespace snowpulse

#endif
