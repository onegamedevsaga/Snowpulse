#include "action_interpolate.h"

#include "../../common/math_utils.h"

namespace snowpulse {
ActionInterpolate* ActionInterpolate::Create(float target, float start, float duration, Easing easing) {
    auto action = new ActionInterpolate();
    action->target_ = target;
    action->start_ = start;
    action->value_ = action->start_;
    action->duration_ = duration;
    action->easing_ = easing;
    action->startTime_ = duration;
    return action;
}

ActionInterpolate::ActionInterpolate() : Action("action_interpolate"), onUpdate_(SPNULL) {
}

ActionInterpolate::~ActionInterpolate() {
}

void ActionInterpolate::Update(float deltaTime) {
    duration_ = MathUtils::Max(duration_ - deltaTime, 0.0f);
    float percentage = (startTime_ - duration_) / startTime_;
    value_ = MathUtils::Ease(start_, target_, percentage, easing_);
    if (onUpdate_) {
        onUpdate_(value_, this);
    }

    if (duration_ == 0.0f) {
        isComplete_ = true;
        if (onComplete_) {
            onComplete_(this);
        }
    }
}

ActionInterpolate* ActionInterpolate::OnUpdate(std::function<void(float, ActionInterpolate*)> onUpdate) {
    if (onUpdate) {
        onUpdate_ = onUpdate;
    }
    return this;
}
}   // namespace snowpulse
