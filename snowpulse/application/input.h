#ifndef SNOWPULSE_APPLICATION_INPUT_H_
#define SNOWPULSE_APPLICATION_INPUT_H_

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "../common/vector2.h"
#include "../common/touch.h"

namespace snowpulse {
class SNOWPULSEAPI Input {
    public:
        static Input* GetInstance();

        virtual ~Input();

        virtual void ClearLastFrameData();
        virtual bool GetPressed(std::string key) = 0;
        virtual bool GetReleased(std::string key) = 0;

        Vector2 GetMousePosition() { return mousePosition_; }
        std::vector<Touch> GetTouches();

    protected:
        Input();
        std::string ToLowerCase(const std::string& str);
        int GetNextAvailableTouchId();

        Vector2 mousePosition_;
        std::map<int, Touch> touches_;
        std::map<void*, int> touchIds_;

};
}   // namespace snowpulse
#endif
