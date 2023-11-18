#ifndef SNOWPULSE_COMMON_VERTEX_H_
#define SNOWPULSE_COMMON_VERTEX_H_

#include "../defines.h"

#include "vector3.h"
#include "vector2.h"
#include "color.h"

namespace snowpulse {
class SNOWPULSEAPI Vertex {
    public:
        Vertex(Vector3 position, Vector2 uv, Color color) : position(position), uv(uv), color(color) {
        }

        Vector3 position;
        Vector2 uv;
        Color color;
};
}   // namespace snowpulse
#endif
