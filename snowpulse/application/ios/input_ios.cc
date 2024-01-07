#include "input_ios.h"

#include <imgui.h>

namespace snowpulse {

InputIOS::InputIOS() : Input() {
}

InputIOS::~InputIOS() {
}

void InputIOS::ProcessInputs(const Vector2Int& resolutionSize, const Vector2Int& screenSize, void* touch, const TouchPhase touchPhase, const Vector2Int& position) {
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
    Vector2 scaleFactor = Vector2((float)resolutionSize.x / (float)screenSize.x, (float)resolutionSize.y / (float)screenSize.y);

    mousePositionOnScreen_ = pos;
    prevMousePositionOnScreen_ = mousePositionOnScreen_;

    auto camPos = camera_->GetPosition();
    mousePositionOnWorld_ = (mousePositionOnScreen_ * scaleFactor) + Vector2(camPos.x, camPos.y) + Vector2(resolutionSize.x * -0.5f, resolutionSize.y * -0.5f);
    prevMousePositionOnWorld_ = mousePositionOnWorld_;

    std::string mouseKey = "mouse_left";

    if (touchPhase == TouchPhase::kBegan) {
        auto newId = GetNextAvailableTouchId();
        Touch t(newId, touchPhase, mousePositionOnScreen_, mousePositionOnWorld_);
        touchIds_[touch] = newId;
        touches_[newId] = t;

        if (newId == 0) {
            pressedKeys_[mouseKey] = true;
            heldKeys_[mouseKey] = true;
        }
    }
    else if (touchPhase == TouchPhase::kMoved) {
        if (touchIds_.count(touch)) {
            auto touchId = touchIds_[touch];
            Touch t(touchId, touchPhase, mousePositionOnScreen_, mousePositionOnWorld_);
            touches_[touchId] = t;
        }
    }
    else if (touchPhase == TouchPhase::kEnded) {
        if (touchIds_.count(touch)) {
            auto touchId = touchIds_[touch];
            Touch t(touchId, touchPhase, mousePositionOnScreen_, mousePositionOnWorld_);
            touches_[touchId] = t;
            touchIds_.erase(touch);

            if (touchId == 0) {
                releasedKeys_[mouseKey] = true;
                heldKeys_.erase(mouseKey);
            }
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[0] = touchPhase == TouchPhase::kBegan || touchPhase == TouchPhase::kMoved;
    io.MousePos = ImVec2(position.x, position.y);
}

void InputIOS::ClearLastFrameData() {
    Input::ClearLastFrameData();
    ImGuiIO& io = ImGui::GetIO();
    for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) {
        io.KeysDown[i] = false;
    }
}

bool InputIOS::GetPressed(std::string key) {
    return pressedKeys_.count(ToLowerCase(key));
}

bool InputIOS::GetDown(std::string key) {
    return heldKeys_.count(ToLowerCase(key));
}

bool InputIOS::GetReleased(std::string key) {
    return releasedKeys_.count(ToLowerCase(key));
}
}   // namespace snowpulse
