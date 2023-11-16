#ifndef SNOWPULSE_GAME_ACTIONMANAGER_H_
#define SNOWPULSE_GAME_ACTIONMANAGER_H_

#include "../defines.h"
#include "../common/singleton.h"

#include <memory>
#include <vector>
#include <functional>

#include "updatable.h"
#include "action.h"

namespace snowpulse {

class SNOWPULSEAPI ActionManager : public Updatable {
    public:
        static std::shared_ptr<ActionManager> Create();

        virtual ~ActionManager();

        // Updatable
        void Start();
        void Update(float deltaTime);

        void RunAction(Action* action);

    private:
        ActionManager();
        void RemoveAction(Action* action);

        std::vector<std::shared_ptr<Action>> actions_;
};
}   // namespace snowpulse

#endif
