#ifndef SNOWPULSE_GAME_ACTIONS_ACTIONMOVE_H_
#define SNOWPULSE_GAME_ACTIONS_ACTIONMOVE_H_

#include "../../defines.h"
#include "../action.h"

#include "../../common/space.h"
#include "../../common/easing.h"
#include "../../common/vector2.h"
#include "../components/transform.h"

namespace snowpulse {

class SNOWPULSEAPI ActionMove : public Action {
    public:
        static ActionMove* Create(Vector2 target, Vector2 start, Transform* transform, float duration, Space space = Space::kLocal, Easing easing = Easing::kLinear);

        ~ActionMove();
        void Update(float deltaTime);

    private:
        ActionMove();

        Vector2 target_;
        Vector2 start_;
        Transform* transform_;
        float startTime_;
        float duration_;
        Space space_;
        Easing easing_;
};
}   // namespace snowpulse
#endif
