#include "action_wait.h"

#include "../../common/math_utils.h"

namespace snowpulse {
ActionWait* ActionWait::Create(float duration) {
    auto action = new ActionWait();
    action->duration_ = duration;
    return action;
}

ActionWait::ActionWait() : Action("action_wait") {
}

ActionWait::~ActionWait() {
}

void ActionWait::Update(float deltaTime) {
    duration_ = MathUtils::Max(duration_ - deltaTime, 0.0f);
    if (duration_ <= 0.0f) {
        isComplete_ = true;
        if (onComplete_) {
            onComplete_(this);
        }
    }
}
}   // namespace snowpulse
