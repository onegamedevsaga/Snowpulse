#include "input.h"

#ifdef SNOWPULSE_PLATFORM_WINDOWS
#include "opengl/input_opengl.h"
#elif SNOWPULSE_PLATFORM_MACOS
#include "opengl/input_opengl.h"
#elif SNOWPULSE_PLATFORM_IOS
//#include "opengl/input_opengl.h"
#endif

namespace snowpulse {
Input* Input::GetInstance() {
#ifdef SNOWPULSE_PLATFORM_WINDOWS
    return InputOpenGL::GetSingletonInstance();
#elif SNOWPULSE_PLATFORM_MACOS
    return InputOpenGL::GetSingletonInstance();
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

std::string Input::ToLowerCase(const std::string& str) {
    auto result = str;
    for (char& c : result) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return result;
}
}   // namespace snowpulse
