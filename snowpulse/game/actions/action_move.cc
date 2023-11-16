#include "action_move.h"

#include "../../common/math_utils.h"

namespace snowpulse {
ActionMove* ActionMove::Create(Vector2 target, Vector2 start, Transform* transform, float duration, Space space, Easing easing) {
    auto action = new ActionMove();
    action->target_ = target;
    action->start_ = start;
    action->transform_ = transform;
    action->duration_ = duration;
    action->space_ = space;
    action->easing_ = easing;
    action->startTime_ = duration;

    switch (action->space_) {
        case Space::kLocal:
            action->transform_->SetLocalPosition(action->start_);
            break;
        case Space::kWorld:
            action->transform_->SetPosition(action->start_);
            break;
    }
    return action;
}

ActionMove::ActionMove() : Action("action_move") {
}

ActionMove::~ActionMove() {
}

void ActionMove::Update(float deltaTime) {
    duration_ = MathUtils::Max(duration_ - deltaTime, 0.0f);

    float percentage = (startTime_ - duration_) / startTime_;
    switch (space_) {
        case Space::kLocal:
            transform_->SetLocalPosition(MathUtils::Ease(start_, target_, percentage, easing_));
            break;
        case Space::kWorld:
            transform_->SetPosition(MathUtils::Ease(start_, target_, percentage, easing_));
            break;
    }

    if (duration_ == 0.0f) {
        isComplete_ = true;
        if (onComplete_) {
            onComplete_(this);
        }
    }
}
}   // namespace snowpulse
