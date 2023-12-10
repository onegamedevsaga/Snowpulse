#ifndef SNOWPULSE_APPLICATION_IOS_INPUTIOS_H_
#define SNOWPULSE_APPLICATION_IOS_INPUTIOS_H_

#include "../../common/singleton.h"
#include "../input.h"

#include <string>
#include <map>

#include "../../common/vector2.h"
#include "../../common/vector2int.h"

namespace snowpulse {
class SNOWPULSEAPI InputIOS : public Input, public Singleton<InputIOS> {
    public:
        virtual ~InputIOS();

        void ProcessInputs(const Vector2Int& resolution, const Vector2Int& screenSize, void* touch, const TouchPhase touchPhase, const Vector2Int& position);
        bool GetPressed(std::string key) override { return false; }
        bool GetDown(std::string key) override { return false; }
        bool GetReleased(std::string key) override { return false; }

    protected:
        friend class Singleton<InputIOS>;
        InputIOS();
};
}   // namespace snowpulse

#endif
