#ifndef SNOWPULSE_COMMON_TOUCH_H_
#define SNOWPULSE_COMMON_TOUCH_H_

#include "../defines.h"

#include "vector2.h"
#include "touch_phase.h"

namespace snowpulse {
class SNOWPULSEAPI Touch {
    public:
        Touch() { }
        Touch(int touchId, TouchPhase phase, Vector2 screenLocation, Vector2 worldLocation) : id_(touchId), phase_(phase), screenLocation_(screenLocation), worldLocation_(worldLocation) { }

        int GetId() const { return id_; }
        TouchPhase GetPhase() const { return phase_; }
        Vector2 GetScreenLocation() const { return screenLocation_; }
        Vector2 GetWorldLocation() const { return worldLocation_; }

    private:
        int id_;
        TouchPhase phase_;
        Vector2 screenLocation_;
        Vector2 worldLocation_;
};
}   // namespace snowpulse
#endif
