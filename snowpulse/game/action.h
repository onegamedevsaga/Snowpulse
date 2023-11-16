#ifndef SNOWPULSE_GAME_ACTION_H_
#define SNOWPULSE_GAME_ACTION_H_

#include "../defines.h"

#include <memory>
#include <string>
#include <functional>

#include "updatable.h"

namespace snowpulse {

class SNOWPULSEAPI Action : public Updatable {
    public:
        virtual ~Action();
        virtual void Update(float deltaTime) = 0;

        std::string GetName() const { return name_; }
        bool IsComplete() const { return isComplete_; }

        void Run();
        Action* OnComplete(std::function<void(Action*)> onComplete);

    protected:
        Action(std::string name);

        std::string name_;
        bool isComplete_;
        std::function<void(Action*)> onComplete_;
};
}   // namespace snowpulse
#endif
