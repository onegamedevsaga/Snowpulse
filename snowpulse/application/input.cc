#include "input.h"

#include <cctype>

#ifdef SNOWPULSE_PLATFORM_WINDOWS
#include "opengl/input_opengl.h"
#elif SNOWPULSE_PLATFORM_MACOS
#include "macos/input_macos.h"
#elif SNOWPULSE_PLATFORM_IOS
#include "ios/input_ios.h"
#endif

namespace snowpulse {
Input* Input::GetInstance() {
#ifdef SNOWPULSE_PLATFORM_WINDOWS
    return InputOpenGL::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_MACOS
    return InputMacOS::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_ANDROID
    
#elif SNOWPULSE_PLATFORM_IOS
    return InputIOS::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_WEBGL
    
#endif
}

Input::Input() {
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
