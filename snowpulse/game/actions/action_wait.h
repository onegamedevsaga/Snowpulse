#ifndef SNOWPULSE_GAME_ACTIONS_ACTIONWAIT_H_
#define SNOWPULSE_GAME_ACTIONS_ACTIONWAIT_H_

#include "../../defines.h"
#include "../action.h"

namespace snowpulse {

class SNOWPULSEAPI ActionWait : public Action {
    public:
        static ActionWait* Create(float duration);

        ~ActionWait();
        void Update(float deltaTime);

    private:
        ActionWait();

        float duration_;
};
}   // namespace snowpulse
#endif
