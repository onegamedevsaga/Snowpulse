#ifndef SNOWPULSE_COMMON_COLOR_H_
#define SNOWPULSE_COMMON_COLOR_H_

#include "../defines.h"

#include <string>

namespace snowpulse {
class SNOWPULSEAPI Color {
    public:
        static Color White()        { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
        static Color Black()        { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
        static Color Red()          { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
        static Color Green()        { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
        static Color Blue()         { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
        static Color Transparent()  { return Color(1.0f, 1.0f, 1.0f, 0.0f); }

        Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
        Color(std::string hex);

        Color operator+(const Color& other) const {
            return Color(r + other.r, g + other.g, b + other.b, a + other.a);
        }

        Color operator-(const Color& other) const {
            return Color(r - other.r, g - other.g, b - other.b, a - other.a);
        }

        Color operator*(float scalar) const {
            return Color(r * scalar, g * scalar, b * scalar, a * scalar);
        }

        float r;
        float g;
        float b;
        float a;
};
}   // namespace snowpulse

#endif
