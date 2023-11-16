#ifndef SNOWPULSE_COMMON_VECTOR2_H_
#define SNOWPULSE_COMMON_VECTOR2_H_

#include "../defines.h"

#include <cmath>

namespace snowpulse {
class SNOWPULSEAPI Vector2 {
    public:
        Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

        Vector2 operator+(const Vector2& other) const {
            return Vector2(x + other.x, y + other.y);
        }

        Vector2 operator-(const Vector2& other) const {
            return Vector2(x - other.x, y - other.y);
        }

        Vector2 operator*(float scalar) const {
            return Vector2(x * scalar, y * scalar);
        }

        Vector2 operator*(const Vector2& other) const {
            return Vector2(x * other.x, y * other.y);
        }

        Vector2& operator=(const Vector2& other) {
            if (this == &other) return *this;
            x = other.x;
            y = other.y;
            return *this;
        }

        Vector2& operator+=(const Vector2& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2& operator-=(const Vector2& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2& operator*=(const Vector2& other) {
            x *= other.x;
            y *= other.y;
            return *this;
        }

        Vector2& operator/=(const Vector2& other) {
            x /= other.x;
            y /= other.y;
            return *this;
        }

        bool operator==(const Vector2& other) const {
            return (x == other.x && y == other.y);
        }

        bool operator!=(const Vector2& other) const {
            return !(*this == other);
        }

        float magnitude() const {
            return std::sqrt(x * x + y * y);
        }

        Vector2 normalized() const {
            float mag = magnitude();
            if (mag != 0.0) {
                return Vector2(x / mag, y / mag);
            }
            else {
                return Vector2();
            }
        }

        float x;
        float y;
};
}   // namespace snowpulse
#endif
