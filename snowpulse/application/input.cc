#include "input.h"

#include <limits>

#include "application.h"

#ifdef SNOWPULSE_PLATFORM_WINDOWS
#include "windows/input_windows.h"
#elif SNOWPULSE_PLATFORM_MACOS
#include "macos/input_macos.h"
#elif SNOWPULSE_PLATFORM_IOS
#include "ios/input_ios.h"
#endif

namespace snowpulse {
Input* Input::GetInstance() {
#ifdef SNOWPULSE_PLATFORM_WINDOWS
    return InputWindows::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_MACOS
    return InputMacOS::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_ANDROID
    
#elif SNOWPULSE_PLATFORM_IOS
    return InputIOS::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_WEBGL
    
#endif
}

Input::Input() {
    floatMaxValue_ = std::numeric_limits<float>::max();
    application_ = Application::GetInstance();
    camera_ = application_->GetGraphics()->GetCamera();

    mousePositionOnWorld_ = Vector2(floatMaxValue_, floatMaxValue_);
    mousePositionOnScreen_ = Vector2(floatMaxValue_, floatMaxValue_);
    prevMousePositionOnWorld_ = mousePositionOnWorld_;
    prevMousePositionOnScreen_ = mousePositionOnScreen_;
}

Input::~Input() {
}

std::vector<Touch> Input::GetTouches() {
    std::vector<Touch> touches;
    for (auto pair : touches_) {
        touches.push_back(pair.second);
    }
    return touches;
}

void Input::ClearLastFrameData() {
    touches_.clear();
    pressedKeys_.clear();
    releasedKeys_.clear();
    scrollDelta_.x = 0.0f;
    scrollDelta_.y = 0.0f;
    mousePositionOnWorld_ = prevMousePositionOnWorld_;
    mousePositionOnScreen_ = prevMousePositionOnScreen_;
}

void Input::ConsumeInputs() {
    touches_.clear();
    pressedKeys_.clear();
    releasedKeys_.clear();
    scrollDelta_.x = 0.0f;
    scrollDelta_.y = 0.0f;

    prevMousePositionOnWorld_ = mousePositionOnWorld_;
    prevMousePositionOnScreen_ = mousePositionOnScreen_;
    mousePositionOnWorld_ = Vector2(floatMaxValue_, floatMaxValue_);
    mousePositionOnScreen_ = Vector2(floatMaxValue_, floatMaxValue_);
}

std::string Input::ToLowerCase(const std::string& str) {
    auto result = str;
    for (char& c : result) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return result;
}

int Input::GetNextAvailableTouchId() {
    int i = 0;
    for (auto pair = touchIds_.begin(); pair != touchIds_.end(); pair++) {
        i = pair->second >= i ? pair->second + 1 : i;
    }
    return i;
}
}   // namespace snowpulse
