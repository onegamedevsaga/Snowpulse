#ifndef SNOWPULSE_GAME_ACTIONS_ACTIONINTERPOLATE_H_
#define SNOWPULSE_GAME_ACTIONS_ACTIONINTERPOLATE_H_

#include "../../defines.h"
#include "../action.h"

#include <functional>

#include "../../common/easing.h"

namespace snowpulse {

class SNOWPULSEAPI ActionInterpolate : public Action {
    public:
        static ActionInterpolate* Create(float target, float start, float duration, Easing easing = Easing::kLinear);

        ~ActionInterpolate();
        void Update(float deltaTime);
        ActionInterpolate* OnUpdate(std::function<void(float, ActionInterpolate*)> onUpdate);

    private:
        ActionInterpolate();

        float value_;
        float target_;
        float start_;
        float startTime_;
        float duration_;
        Easing easing_;
        std::function<void(float, ActionInterpolate*)> onUpdate_;
};
}   // namespace snowpulse
#endif
