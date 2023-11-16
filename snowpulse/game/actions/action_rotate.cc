#include "action_rotate.h"

#include "../../common/math_utils.h"

namespace snowpulse {
ActionRotate* ActionRotate::Create(float target, float start, Transform* transform, float duration, Space space, Easing easing) {
    auto action = new ActionRotate();
    action->target_ = target;
    action->start_ = start;
    action->transform_ = transform;
    action->duration_ = duration;
    action->space_ = space;
    action->easing_ = easing;
    action->startTime_ = duration;

    switch (action->space_) {
        case Space::kLocal:
            action->transform_->SetLocalRotation(action->start_);
            break;
        case Space::kWorld:
            action->transform_->SetRotation(action->start_);
            break;
    }
    return action;
}

ActionRotate::ActionRotate() : Action("action_rotate") {
}

ActionRotate::~ActionRotate() {
}

void ActionRotate::Update(float deltaTime) {
    duration_ = MathUtils::Max(duration_ - deltaTime, 0.0f);

    float percentage = (startTime_ - duration_) / startTime_;
    switch (space_) {
        case Space::kLocal:
            transform_->SetLocalRotation(MathUtils::Ease(start_, target_, percentage, easing_));
            break;
        case Space::kWorld:
            transform_->SetRotation(MathUtils::Ease(start_, target_, percentage, easing_));
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
