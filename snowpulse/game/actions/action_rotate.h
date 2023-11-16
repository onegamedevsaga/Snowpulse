#ifndef SNOWPULSE_GAME_ACTIONS_ACTIONROTATE_H_
#define SNOWPULSE_GAME_ACTIONS_ACTIONROTATE_H_

#include "../../defines.h"
#include "../action.h"

#include "../../common/space.h"
#include "../../common/easing.h"
#include "../components/transform.h"

namespace snowpulse {

class SNOWPULSEAPI ActionRotate : public Action {
    public:
        static ActionRotate* Create(float target, float start, Transform* transform, float duration, Space space = Space::kLocal, Easing easing = Easing::kLinear);

        ~ActionRotate();
        void Update(float deltaTime);

    private:
        ActionRotate();

        float target_;
        float start_;
        Transform* transform_;
        float startTime_;
        float duration_;
        Space space_;
        Easing easing_;
};
}   // namespace snowpulse
#endif
