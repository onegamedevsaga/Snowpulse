#ifndef SNOWPULSE_APPLICATION_INPUT_H_
#define SNOWPULSE_APPLICATION_INPUT_H_

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "../common/vector2.h"
#include "../common/touch.h"
#include "../application/camera.h"

namespace snowpulse {

// Forward declaration
class Application;

class SNOWPULSEAPI Input {
    public:
        static Input* GetInstance();

        virtual ~Input();

        virtual void ClearLastFrameData();
        virtual bool GetPressed(std::string key) = 0;
        virtual bool GetDown(std::string key) = 0;
        virtual bool GetReleased(std::string key) = 0;

        Vector2 GetMousePositionOnScreen() { return mousePositionOnScreen_; }
        Vector2 GetMousePositionOnWorld() { return mousePositionOnWorld_; }
        std::vector<Touch> GetTouches();

    protected:
        Input();
        std::string ToLowerCase(const std::string& str);
        int GetNextAvailableTouchId();

        Application* application_;
        Camera* camera_;

        std::map<int, Touch> touches_;
        std::map<void*, int> touchIds_;

        Vector2 mousePositionOnScreen_;
        Vector2 mousePositionOnWorld_;
        Vector2 scrollDelta_;
        std::map<unsigned int, std::string> keyMap_;
        std::map<std::string, bool> pressedKeys_;
        std::map<std::string, bool> releasedKeys_;
        std::map<std::string, bool> heldKeys_;

};
}   // namespace snowpulse
#endif
