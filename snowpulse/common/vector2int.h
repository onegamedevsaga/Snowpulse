#ifndef SNOWPULSE_COMMON_Vector2IntINT_H_
#define SNOWPULSE_COMMON_Vector2IntINT_H_

#include "../defines.h"

#include <cmath>

namespace snowpulse {
class SNOWPULSEAPI Vector2Int {
    public:
        Vector2Int(int x = 0.0f, int y = 0.0f) : x(x), y(y) {}

        Vector2Int operator+(const Vector2Int& other) const {
            return Vector2Int(x + other.x, y + other.y);
        }

        Vector2Int operator-(const Vector2Int& other) const {
            return Vector2Int(x - other.x, y - other.y);
        }

        Vector2Int operator*(int scalar) const {
            return Vector2Int(x * scalar, y * scalar);
        }

        Vector2Int operator*(const Vector2Int& other) const {
            return Vector2Int(x * other.x, y * other.y);
        }

        Vector2Int& operator=(const Vector2Int& other) {
            if (this == &other) return *this;
            x = other.x;
            y = other.y;
            return *this;
        }

        bool operator==(const Vector2Int& other) const {
            return (x == other.x && y == other.y);
        }

        bool operator!=(const Vector2Int& other) const {
            return !(*this == other);
        }

        int magnitude() const {
            return std::sqrt(x * x + y * y);
        }

        Vector2Int normalized() const {
            int mag = magnitude();
            if (mag != 0.0) {
                return Vector2Int(x / mag, y / mag);
            }
            else {
                return Vector2Int();
            }
        }

        int x;
        int y;
};
}   // namespace snowpulse
#endif
