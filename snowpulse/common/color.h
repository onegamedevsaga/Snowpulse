#ifndef SNOWPULSE_COMMON_COLOR_H_
#define SNOWPULSE_COMMON_COLOR_H_

#include "../defines.h"

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

        float r;
        float g;
        float b;
        float a;
};
}   // namespace snowpulse

#endif
