#include "color.h"

#include <stdexcept>

namespace snowpulse {
Color::Color(std::string hex) {
    if (hex.length() != 9 || hex[0] != '#') {
        throw std::invalid_argument("Invalid hex format");
    }

    float multiplier = 1.0f / 255.0f;
    int ir, ig, ib, ia;
    sscanf(hex.c_str(), "#%02x%02x%02x%02x", &ir, &ig, &ib, &ia);

    r = (float)ir * multiplier;
    g = (float)ig * multiplier;
    b = (float)ib * multiplier;
    a = (float)ia * multiplier;
}
}   // namespace snowpulse
