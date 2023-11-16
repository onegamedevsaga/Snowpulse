#include "input.h"

namespace snowpulse {
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
