#ifndef SNOWPULSE_APPLICATION_INPUT_H_
#define SNOWPULSE_APPLICATION_INPUT_H_

#include <memory>
#include <string>
#include <cctype>

#include "../common/vector2.h"

namespace snowpulse {
class SNOWPULSEAPI Input {
    public:
        static Input* GetInstance();

        virtual ~Input();

        virtual bool GetPressed(std::string key) = 0;
        virtual bool GetReleased(std::string key) = 0;
        virtual Vector2 GetInputPosition() = 0;

    protected:
        Input();

        std::string ToLowerCase(const std::string& str);

};
}   // namespace snowpulse
#endif
