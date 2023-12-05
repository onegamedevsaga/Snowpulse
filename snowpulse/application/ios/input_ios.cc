#include "input_ios.h"

namespace snowpulse {

InputIOS::InputIOS() : Input() {
}

InputIOS::~InputIOS() {
}

void InputIOS::ProcessInputs(const Vector2Int& resolution, const Vector2Int& screenSize, void* touch, const TouchPhase touchPhase, const Vector2Int& position) {
    auto copy = touches_;
    for (auto pair : copy) {
        bool found = false;
        for (auto pair2 : touchIds_) {
            if (pair2.second == pair.first) {
                found = true;
                break;
            }
        }
        if (!found) {
            touches_.erase(pair.first);
        }
    }

    Vector2 pos;
    pos.x = (float)position.x;
    pos.y = (float)screenSize.y - (float)position.y;
    Vector2 scaleFactor = Vector2((float)resolution.x / (float)screenSize.x, (float)resolution.y / (float)screenSize.y);
    pos *= scaleFactor;

    if (touchPhase == TouchPhase::kBegan) {
        auto newId = GetNextAvailableTouchId();
        Touch t(newId, touchPhase, pos);
        touchIds_[touch] = newId;
        touches_[newId] = t;
    }
    else if (touchPhase == TouchPhase::kMoved) {
        if (touchIds_.count(touch)) {
            auto touchId = touchIds_[touch];
            Touch t(touchId, touchPhase, pos);
            touches_[touchId] = t;
        }
    }
    else if (touchPhase == TouchPhase::kEnded) {
        if (touchIds_.count(touch)) {
            auto touchId = touchIds_[touch];
            Touch t(touchId, touchPhase, pos);
            touches_[touchId] = t;
            touchIds_.erase(touch);
        }
    }
}
}   // namespace snowpulse
