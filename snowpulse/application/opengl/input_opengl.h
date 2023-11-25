#ifndef SNOWPULSE_APPLICATION_OPENGL_INPUTOPENGL_H_
#define SNOWPULSE_APPLICATION_OPENGL_INPUTOPENGL_H_

#include "../../common/singleton.h"
#include "../input.h"

#include <string>
#include <map>

#include "../../common/vector2.h"
#include "graphics_opengl.h"

namespace snowpulse {
class SNOWPULSEAPI InputOpenGL : public Input, public Singleton<InputOpenGL> {
    public:
        virtual ~InputOpenGL();

        void ProcessInputs(GraphicsOpenGL* graphics);
        bool GetPressed(std::string key) override;
        bool GetReleased(std::string key) override;
        Vector2 GetInputPosition() override { return inputPosition_; }

    protected:
        friend class Singleton<InputOpenGL>;
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
