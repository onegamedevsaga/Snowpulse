#ifndef SNOWPULSE_COMMON_TOUCH_H_
#define SNOWPULSE_COMMON_TOUCH_H_

#include "../defines.h"

#include "vector2.h"
#include "touch_phase.h"

namespace snowpulse {
class SNOWPULSEAPI Touch {
    public:
        Touch() { }
        Touch(int touchId, TouchPhase phase, Vector2 position) : id_(touchId), phase_(phase), position_(position) { }

        int GetId() const { return id_; }
        TouchPhase GetPhase() const { return phase_; }
        Vector2 GetPosition() const { return position_; }

    private:
        int id_;
        TouchPhase phase_;
        Vector2 position_;
};
}   // namespace snowpulse
#endif
